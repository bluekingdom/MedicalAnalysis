#include "BUAnalysis.h"
#include "Algorithm/Common/glog.h"
#include "Algorithm/Common/Utils.h"

namespace SYY {
	namespace MedicalAnalysis {

		BUAnalysis::BUAnalysis()
			:m_pBUDetector(nullptr)
			, m_pValidRegionDetector(nullptr)
			, m_pGradingClassifier(nullptr)
			, m_pLessionClassifier(nullptr)
			, m_nCropVersion(0)
			, m_nAnalysisMethod(None)
		{
		}

		ErrorCode BUAnalysis::Analysis(const cv::Mat& srcImg, BUAnalysisResult& result)
		{
			GLOG("BUAnalysis::Analysis info: enter!\n");

			//if (result.pLessionRects)
			//{
			//	delete[] result.pLessionRects;
			//	delete[] result.pLessionConfidence;
			//	delete[] result.pLessionTypes;

			//	result.pLessionRects = nullptr;
				result.nLessionsCount = 0;
			//}

			if (srcImg.empty())
			{
				GLOG("BUAnalysis::Analysis error: srcImg is empty!\n");
				return SYY_SYS_ERROR;
			}

			// get valid rect
			cv::Rect validRect;

			CropValidRegion(srcImg, validRect);

			result.rCropRect = Common::CVRect2Rect(validRect);

			auto validImg = srcImg(validRect);

			// classify grade
			std::vector<int> grades;
			if (false == m_pGradingClassifier->Classify(validImg, grades))
			{
				GLOG("ERROR: grade classify error!\n");
				return SYY_SYS_ERROR;
			}
			result.nGrading = (LessionGrading)grades[0];

			// detect lession
			std::vector<DeepLearning::Detect_Result> detections;

			switch (m_nAnalysisMethod)
			{
			case SYY::MedicalAnalysis::BUAnalysis::None:
				GLOG("Error: Please Init first!\n");
				return SYY_SYS_ERROR;
			case SYY::MedicalAnalysis::BUAnalysis::FRCNN:
				if (false == AnalysisFRCNN(validImg, detections))
					return SYY_SYS_ERROR;
				break;
			case SYY::MedicalAnalysis::BUAnalysis::SSD:
			case SYY::MedicalAnalysis::BUAnalysis::SSD_MULTI:
				if (false == AnalysisSSD(srcImg, validRect, detections))
					return SYY_SYS_ERROR;
				break;
			default:
				GLOG("Error: Unknown Analysis Method!\n");
				return SYY_SYS_ERROR;
			}

			float threshold = 0.10f;

			std::vector<Rect> rects;
			std::vector<float> confs;
			for (auto detect : detections)
			{
				if (detect.score < threshold)
					continue;

				Rect r;
				r.x = detect.bbox[0];
				r.y = detect.bbox[1];
				r.w = detect.bbox[2] - detect.bbox[0];
				r.h = detect.bbox[3] - detect.bbox[1];

				r.x += validRect.x;
				r.y += validRect.y;

				r.x = std::max(0, r.x);
				r.y = std::max(0, r.y);
				r.w = std::min(srcImg.cols - r.x, r.w);
				r.h = std::min(srcImg.rows - r.y, r.h);

				rects.push_back(r);
				confs.push_back(detect.score);
			}

			result.nLessionsCount = std::min((int)rects.size(), BUAnalysisResult::MAX_LEN);

			if (result.nLessionsCount > 0)
			{
				// lession type classify
				std::vector<cv::Mat> imgs;
				for (auto rect : rects) imgs.push_back(srcImg(Common::Rect2CVRect(rect)));

				std::vector<DeepLearning::Classify_Result> cls_res_batch;
				if (false == m_pLessionClassifier->Classify_Batch(imgs, cls_res_batch))
				{
					GLOG("Error: m_pLessionClassifier->Classify_Batch fail!\n");
					return SYY_SYS_ERROR;
				}

				// set result
				//result.pLessionRects = new Rect[result.nLessionsCount];
				//result.pLessionConfidence = new float[result.nLessionsCount];
				//result.pLessionTypes = new LessionType[result.nLessionsCount];

				for (int i = 0; i < result.nLessionsCount; i++)
				{
					result.pLessionRects[i] = rects[i];
					result.pLessionConfidence[i] = confs[i];
					result.pLessionTypes[i] = (LessionType)cls_res_batch[i].vOutput[0];
				}
			}

			//auto drawing = srcImg.clone();
			//cv::rectangle(drawing, validRect, cv::Scalar(255, 255, 255));
			//cv::imshow("drawing", drawing);
			//cv::waitKey();

			GLOG("BUAnalysis::Analysis info: exit!\n");
			return SYY_NO_ERROR;
		}

		bool CheckFile(const std::string& file) {
			if (false == FileSystem::IsExists(file))
			{
				GLOG("file not exist: %s\n", file.c_str());
				return false;
			}
			return true;
		};

		ErrorCode BUAnalysis::Init(unsigned long nMode)
		{
			if (!m_pBUDetector)
				m_pBUDetector = new DeepLearning;

			if (!m_pValidRegionDetector)
				m_pValidRegionDetector = new DeepLearning;

			const int gpu_idx = -1;

			bool res = false;

			if (nMode & BUAnalysisMode::DetectMore)
			{
				res = InitSSDMore();
			}
			else if (nMode & BUAnalysisMode::DetectAccurate)
			{
				res = InitSSDAccurate();
			}

			if (false == res)
			{
				return SYY_SYS_ERROR;
			}

			if (nMode & BUAnalysisMode::Crop_V1)
			{
				m_nCropVersion = 1;
			}
			else if (nMode & BUAnalysisMode::Crop_V2)
			{
				m_nCropVersion = 2;

				if (false == CropInit_V2())
				{
					return SYY_SYS_ERROR;
				}

			}
			else if (nMode & BUAnalysisMode::Crop_V3)
			{
				m_nCropVersion = 3;

				if (false == CropInit_V3())
				{
					return SYY_SYS_ERROR;
				}
			}

			if (false == InitGradeClassifier())
			{
				return SYY_SYS_ERROR;
			}

			if (false == InitLessionClassifier())
			{
				return SYY_SYS_ERROR;
			}

			return SYY_NO_ERROR;
		}

		ErrorCode BUAnalysis::Release()
		{
			switch (m_nAnalysisMethod)
			{
			case SYY::MedicalAnalysis::BUAnalysis::FRCNN:
				ReleaseFRCNN();
				break;
			case SYY::MedicalAnalysis::BUAnalysis::SSD:
				ReleaseSSD();
				break;
			}

			if (m_pValidRegionDetector)
			{
				if (m_nCropVersion == 2)
					m_pValidRegionDetector->Release_FRCNN();
				else 
					m_pValidRegionDetector->Release();

				delete m_pValidRegionDetector;
			}
			m_pValidRegionDetector = nullptr;

			if (m_pGradingClassifier)
			{
				m_pGradingClassifier->Release();
				delete m_pGradingClassifier;
			}
			m_pGradingClassifier = nullptr;

			if (m_pLessionClassifier)
			{
				m_pLessionClassifier->Release();
				delete m_pLessionClassifier;
			}
			m_pLessionClassifier = nullptr;

			return SYY_NO_ERROR;
		}

		bool BUAnalysis::CropValidRegion_V1(const cv::Mat& srcImg, cv::Rect& validRegion)
		{
			cv::Mat gray, h, v, res1, res2, res3;

			//if (srcImg.size().width > 1024) cv::resize(srcImg, srcImg, cv::Size(1024, int(srcImg.size().height * 1024.f / srcImg.size().width)));

			auto imgSize = srcImg.size();

			cv::cvtColor(srcImg, gray, CV_RGB2GRAY);

			std::vector<cv::Rect> bboxes;

			cv::Mat close_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));
			for (int ksize = 1; ksize <= 21; ksize += 10)
			{
				cv::Mat open_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ksize, ksize));

				cv::morphologyEx(gray, res1, cv::MORPH_OPEN, open_element);
				cv::morphologyEx(res1, res1, cv::MORPH_CLOSE, open_element);

				cv::threshold(res1, res2, 0, 255, cv::THRESH_OTSU);

				GetContoursBBox(res2, bboxes);
			}

			std::vector<cv::Rect> temp;
			const float disable_region_radio = 0.05f;
			cv::Vec4i disable_regions = {
				int(imgSize.width * disable_region_radio),
				int(imgSize.width * (1 - disable_region_radio)),
				int(imgSize.height * disable_region_radio),
				int(imgSize.height * (1 - disable_region_radio)),
			};
			for (auto rect : bboxes)
			{
				float r1 = (float)rect.area() / (float)gray.size().area();
				if (r1 < 0.005f || r1 > 0.9f)
					continue;

				if (rect.x < disable_regions[0] ||
					rect.x + rect.width > disable_regions[1] ||
					rect.y < disable_regions[2] ||
					rect.y + rect.height > disable_regions[3]
					)
					continue;

				temp.push_back(rect);
			}

			bboxes = temp;

			ClusterRegions(gray, bboxes);

			std::vector<std::pair<cv::Rect, int>> rects;
			for (auto rect : bboxes)
			{
				float r1 = (float)rect.area() / (float)gray.size().area();
				if (r1 < 0.025f || r1 > 0.9f)
					continue;

				float r2 = (float)rect.width / (float)rect.height;
				if (r2 < 1.0f || r2 > 5.0f)
					continue;

				int val = cv::sum(res2(rect)).dot(cv::Scalar::ones());
				rects.push_back(std::pair<cv::Rect, int>(rect, val));
			}

			int count = std::min(2, (int)rects.size());
			if (rects.size() >= 2)
			{
				std::partial_sort(rects.begin(), rects.begin() + count, rects.end(),
					[](std::pair<cv::Rect, int> a, std::pair<cv::Rect, int> b)->bool
				{
					float r1 = (float)a.second / (float)a.first.area();
					float r2 = (float)b.second / (float)b.first.area();
					return r1 > r2;
				});
			}

			int i = 0;
			std::vector<cv::Rect> all_rects;
			for (auto p : rects)
			{
				if (i++ >= count)
					break;

				auto rect = p.first;

				float r = (float)rect.height / (float)rect.width;
				float imgR = (float)imgSize.height / (float)imgSize.width;

				if (r < imgR)
				{
					rect.height = int(rect.width * imgR);
					if (rect.y + rect.height >= imgSize.height)
						continue;
				}

				all_rects.push_back(rect);

			}

			if (all_rects.size() == 0)
				return false;

			cv::Rect r = all_rects[0];
			for (int i = 1; i < all_rects.size(); i++)
			{
				r |= all_rects[i];
			}

			validRegion = r;

			return true;
		}

		void BUAnalysis::GetContoursBBox(const cv::Mat& srcImg, std::vector<cv::Rect>& bbox)
		{
			std::vector<std::vector<cv::Point> > contours;
			std::vector<cv::Vec4i> hierarchy;

			auto img = srcImg.clone();
			if (img.channels() == 3)
			{
				cv::cvtColor(img, img, CV_RGB2GRAY);
			}

			cv::findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
			int maxArea = 0;

			//bbox.clear();
			for (int i = 0; i < contours.size(); i++)
			{
				auto h = hierarchy[i];
				if (h[3] != -1)
					continue;

				auto contour = contours[i];
				auto rect = cv::boundingRect(contour);

				bbox.push_back(rect);
			}
		}

		void BUAnalysis::ClusterRegions(const cv::Mat& srcImg, std::vector<cv::Rect>& regions)
		{
			cv::Point pt1, pt2;
			int c = 1;

			while (1)
			{
				bool isCluster = false;
				std::vector<int> Ids(regions.size());
				for (int i = 0; i < regions.size(); i++)
				{
					if (Ids[i] != 0) continue;
					Ids[i] = c++;

					for (int j = i + 1; j < regions.size(); j++)
					{
						if ((regions[i] & regions[j]).area() > 0)
						{
							Ids[j] = Ids[i];
							isCluster = true;
						}
					}
				}

				if (false == isCluster)
					break;

				std::map<int, cv::Rect> id2rect;
				for (int i = 0; i < regions.size(); i++)
				{
					auto id = Ids[i];
					if (id2rect.find(id) == id2rect.end())
						id2rect[id] = regions[i];
					else
						id2rect[id] |= regions[i];
				}

				regions.clear();
				for (const auto& p : id2rect)
				{
					regions.push_back(p.second);
				}
			}
		}

		bool BUAnalysis::CropValidRegion(const cv::Mat& srcImg, cv::Rect& validRegion)
		{
			if (m_nCropVersion == 0)
				return false;

			bool res = false;
			switch (m_nCropVersion)
			{
			case 1:
				res = CropValidRegion_V1(srcImg, validRegion);
				break;
			case 2:
				res = CropValidRegion_V2(srcImg, validRegion);
				break;
			case 3:
				res = CropValidRegion_V3(srcImg, validRegion);
				break;
			}

			if (res == false)
			{
				validRegion.width = srcImg.cols;
				validRegion.height = srcImg.rows;
			}

			return true;
		}

		bool BUAnalysis::CropValidRegion_V2(const cv::Mat& srcImg, cv::Rect& validRegion)
		{
			if (!m_pValidRegionDetector)
			{
				GLOG("m_pValidRegionDetector is null!\n");
				return false;
			}

			std::vector<DeepLearning::Detect_Result> detections;
			if (false == m_pValidRegionDetector->Detect_FRCNN(srcImg, detections))
			{
				GLOG("Execute frcnn error!\n");
				return false;
			}

			if (detections.size() == 0)
			{
				GLOG("detect valid region fail!\n");
				return false;
			}
			else if (detections.size() >= 2)
			{
				std::sort(detections.begin(), detections.end(), [](DeepLearning::Detect_Result& rst1, DeepLearning::Detect_Result& rst2){
					return rst1.score > rst2.score;
				});
			}

			auto bbox = detections[0].bbox;
			const float r = 0.00;
			int w = bbox[2] - bbox[0];
			int h = bbox[3] - bbox[1];
			int x = std::max(0, std::min(srcImg.cols, int(bbox[0] - w * r)));
			int y = std::max(0, std::min(srcImg.rows, int(bbox[1] - h * r)));
			w = std::min(srcImg.cols - x, int(w * (1 + 2 * r)));
			h = std::min(srcImg.rows - y, int(h * (1 + 2 * r)));
			validRegion = cv::Rect(x, y, w, h);

			// adjust rect
			//std::vector<cv::Rect> bboxes;
			//GetContoursBBox(srcImg(validRegion), bboxes);

			//if (bboxes.size() > 0)
			//{
			//	cv::Rect r = bboxes[0];
			//	for (int i = 1; i < bboxes.size(); i++)
			//	{
			//		r |= bboxes[i];
			//	}

			//	r.x += validRegion.x;
			//	r.y += validRegion.y;

			//	validRegion = r;
			//}
			// end adjust rect

			return true;
		}

		bool BUAnalysis::InitFRCNN()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";
			std::string prototxt = root + "zf_test.prototxt";
			std::string caffemodel = root + "custom_data_zf_faster_rcnn_final.caffemodel";
			//std::string prototxt = root + "vgg16_test.prototxt";
			//std::string caffemodel = root + "custom_data_vgg16_faster_rcnn_final.caffemodel";
			const std::string config_file = root + "custom_data_config.json";

			int gpu_idx = -1;

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel) || false == CheckFile(config_file))
			{
				return false;
			}

			m_pBUDetector->SetBlasThreadNum(s_nNumBlasThread);

			if (!m_pBUDetector || false == m_pBUDetector->Init_FRCNN(prototxt, config_file, caffemodel, gpu_idx))
			{
				GLOG("m_pBUDetector init error!\n");
				return false;
			}
			
			m_nAnalysisMethod = FRCNN;

			return true;
		}

		bool BUAnalysis::AnalysisFRCNN(const cv::Mat& srcImg, std::vector<DeepLearning::Detect_Result>& dets)
		{
			if (!m_pBUDetector || (false == m_pBUDetector->Detect_FRCNN(srcImg, dets)))
			{
				GLOG("Execute frcnn error!\n");
				return false;
			}
			return true;
		}

		void BUAnalysis::ReleaseFRCNN()
		{
			if (m_pBUDetector)
			{
				m_pBUDetector->Release_FRCNN();
				delete m_pBUDetector;
			}
			m_pBUDetector = nullptr;
		}

		bool BUAnalysis::InitSSDMore()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";
			//std::string prototxt = root + "ssd_deploy_vgg_300.prototxt";
			//std::string caffemodel = root + "b-scan_iter_1000.caffemodel";

			//std::string prototxt = root + "ssd_deploy_vgg_300.prototxt";
			std::string prototxt = root + Config::GetConfValue("analysis_more.prototxt");

			//std::string caffemodel = root + "20170629-b-scan_iter_30000.caffemodel";
			//std::string caffemodel = root + "20170627-b-scan-lession_iter_30000.caffemodel";
			std::string caffemodel = root + Config::GetConfValue("analysis_more.caffemodel");
			//std::string caffemodel = root + "20170621-b-scan_iter_59000.caffemodel";

			bool is_gpu= false;

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel)) 
			{
				return false;
			}

			m_pBUDetector->SetBlasThreadNum(s_nNumBlasThread);
			std::vector<int> meanvalue = {102, 117, 123};

			if (!m_pBUDetector || false == m_pBUDetector->Init(prototxt, meanvalue, caffemodel, is_gpu))
			{
				GLOG("m_pBUDetector init error!\n");
				return false;
			}
			
			m_nAnalysisMethod = SSD;

			return true;
		}

		bool BUAnalysis::InitSSDAccurate()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";

			//std::string prototxt = root + "ssd_deploy_vgg_300_accurate.prototxt";
			//std::string caffemodel = root + "20170705-b-scan_iter_240000.caffemodel";

			std::string prototxt = root + Config::GetConfValue("analysis_accurate.prototxt");
			std::string caffemodel = root + Config::GetConfValue("analysis_accurate.caffemodel");

			bool is_gpu= false;

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel))
			{
				return false;
			}

			m_pBUDetector->SetBlasThreadNum(s_nNumBlasThread);
			std::vector<int> meanvalue = {102, 117, 123};

			if (!m_pBUDetector || false == m_pBUDetector->Init(prototxt, meanvalue, caffemodel, is_gpu))
			{
				GLOG("m_pBUDetector init error!\n");
				return false;
			}
			
			m_nAnalysisMethod = SSD_MULTI;

			return true;
		}

		bool BUAnalysis::AnalysisSSD(const cv::Mat& srcImg, const cv::Rect& validRect, std::vector<DeepLearning::Detect_Result>& dets)
		{
			if (m_nAnalysisMethod == SSD)
			{
				if (!m_pBUDetector || false == m_pBUDetector->Detect_SSD(srcImg(validRect), dets))
				{
					GLOG("Error: Detect_SSD fail!\n");
					return false;
				}

				return true;
			}
			
			if (m_nAnalysisMethod == SSD_MULTI)
			{
				float radio = 0.05f;
				int rx = validRect.width * radio;
				int ry = validRect.height * radio;

				auto r = validRect;

				std::vector<cv::Vec2i> offsets{
					{ 0, 0 }, { -rx, 0 }, { rx, 0 }, { 0, -ry }, {0, ry}
				};

				std::vector<cv::Rect> rects;
				for (auto o : offsets) rects.push_back(cv::Rect(r.x + o[0], r.y + o[1], r.width, r.height));

				//std::vector<cv::Rect> rects{ 
				//	{ r }, 
				//	{ r.x - rx, r.y, r.width, r.height },
				//	{ r.x + rx, r.y, r.width, r.height },
				//	{ r.x, r.y - ry, r.width, r.height },
				//	{ r.x, r.y + ry, r.width, r.height },
				//};

				std::vector<cv::Mat> imgs;
				for (auto r : rects)
				{
					r.x = std::max(0, r.x);
					r.y = std::max(0, r.y);
					r.width = std::min(srcImg.cols - r.x, r.width);
					r.height = std::min(srcImg.rows - r.y, r.height);
					imgs.push_back(srcImg(r));
				}

				std::vector<DeepLearning::Detect_Result_Batch> vec_result_batch;
				if (!m_pBUDetector || false == m_pBUDetector->Detect_SSD_Batch(imgs, vec_result_batch))
				{
					GLOG("Error: Detect_SSD_Batch fail!\n");
					return false;
				}

				dets.clear();

				int num_batch = vec_result_batch.size();
				dets = vec_result_batch[0].vDetectRst;
				for (int i = 1; i < num_batch; i++)
				{
					auto offset = offsets[i];
					int num_detect_rst = vec_result_batch[i].vDetectRst.size();
					auto vDetRst = vec_result_batch[i].vDetectRst;
					for (int j = 0; j < num_detect_rst; j++)
					{
						auto bbox = vDetRst[j].bbox;
						cv::Rect a(bbox[0] - offset[0], bbox[1] - offset[1], bbox[2] - bbox[0], bbox[3] - bbox[1]); 
						float max_overlay = 0.f;
						int idx = -1;
						for (int dets_idx = 0; dets_idx < dets.size(); dets_idx++)
						{
							auto bbox = dets[dets_idx].bbox;
							cv::Rect b(bbox[0] - offsets[0][0], bbox[1] - offsets[0][1], bbox[2] - bbox[0], bbox[3] - bbox[1]);

							float l = Common::IOU(a, b);
							if (max_overlay < l && l > 0.5f)
							{
								max_overlay = l;
								idx = dets_idx;
							}
						}

						if (idx >= 0 && vDetRst[j].score > dets[idx].score)
						{
							dets[idx] = vDetRst[j];
						}

					}
				}
				return true;
			}

			GLOG("Error: AnalysisSSD error execute path!\n");
			return false;
		}

		void BUAnalysis::ReleaseSSD()
		{
			if (m_pBUDetector)
			{
				m_pBUDetector->Release();
				delete m_pBUDetector;
			}
			m_pBUDetector = nullptr;
		}

		bool BUAnalysis::CropInit_V2()
		{
			auto root = FileSystem::GetCurExePath() + "\\config\\";
			//auto prototxt = root + "imgBox-test.prototxt";
			//auto caffemodel = root + "imgBox_zf_faster_rcnn_final.caffemodel";
			//auto config_file = root + "custom_data_config.json";

			auto prototxt = root + Config::GetConfValue("crop_v2.prototxt");
			auto caffemodel = root + Config::GetConfValue("crop_v2.caffemodel");
			auto config_file = root + Config::GetConfValue("crop_v2.config");

			int gpu_idx = -1;

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel))
			{
				return false;
			}

			m_pValidRegionDetector->SetBlasThreadNum(s_nNumBlasThread);

			if (!m_pValidRegionDetector ||
				false == m_pValidRegionDetector->Init_FRCNN(prototxt, config_file, caffemodel, gpu_idx))
			{
				GLOG("m_pValidRegionDetector init error!\n");
				return false;
			}
			return true;
		}

		bool BUAnalysis::CropInit_V3()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";

			std::string prototxt = root + "imgBox-ssd-deploy.prototxt";
			std::string caffemodel = root + "imgBox_iter_38000.caffemodel";

			bool is_gpu= false;

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel))
			{
				return false;
			}

			m_pValidRegionDetector->SetBlasThreadNum(s_nNumBlasThread);
			std::vector<int> meanvalue = {102, 117, 123};

			if (!m_pValidRegionDetector || false == m_pValidRegionDetector->Init(prototxt, meanvalue, caffemodel, is_gpu))
			{
				GLOG("m_pValidRegionDetector init error!\n");
				return false;
			}
			
			return true;
		}

		bool BUAnalysis::CropValidRegion_V3(const cv::Mat& srcImg, cv::Rect& validRegion)
		{
			if (!m_pValidRegionDetector)
			{
				GLOG("m_pValidRegionDetector is null!\n");
				return false;
			}

			std::vector<DeepLearning::Detect_Result> detections;
			if (false == m_pValidRegionDetector->Detect_SSD(srcImg, detections))
			{
				GLOG("Execute frcnn error!\n");
				return false;
			}

			if (detections.size() == 0)
			{
				GLOG("detect valid region fail!\n");
				return false;
			}
			else if (detections.size() >= 2)
			{
				std::sort(detections.begin(), detections.end(), [](DeepLearning::Detect_Result& rst1, DeepLearning::Detect_Result& rst2){
					return rst1.score > rst2.score;
				});
			}

			auto bbox = detections[0].bbox;
			const float r = 0.00;
			int w = bbox[2] - bbox[0];
			int h = bbox[3] - bbox[1];
			int x = std::max(0, std::min(srcImg.cols, int(bbox[0] - w * r)));
			int y = std::max(0, std::min(srcImg.rows, int(bbox[1] - h * r)));
			w = std::min(srcImg.cols - x, int(w * (1 + 2 * r)));
			h = std::min(srcImg.rows - y, int(h * (1 + 2 * r)));
			validRegion = cv::Rect(x, y, w, h);


			return true;
		}

		bool BUAnalysis::InitGradeClassifier()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";

			//std::string prototxt = root + "gc_fullimage_deploy.prototxt";
			//std::string caffemodel = root + "gc_fullimage_iter_5000.caffemodel";

			std::string prototxt = root + Config::GetConfValue("grading_classify.prototxt");
			std::string caffemodel = root + Config::GetConfValue("grading_classify.caffemodel");

			bool is_gpu = false;

			return ClassifierInitializer(m_pGradingClassifier, prototxt, caffemodel, is_gpu);
		}

		bool BUAnalysis::InitLessionClassifier()
		{
			const std::string root = FileSystem::GetCurExePath() + "\\config\\";

			//std::string prototxt = root + "lession_classify_trainval.prototxt";
			//std::string caffemodel = root + "lession_classify_iter_50000.caffemodel";

			std::string prototxt = root + Config::GetConfValue("lession_type.prototxt");
			std::string caffemodel = root + Config::GetConfValue("lession_type.caffemodel");

			bool is_gpu = false;

			return ClassifierInitializer(m_pLessionClassifier, prototxt, caffemodel, is_gpu);
		}

		bool BUAnalysis::ClassifierInitializer(DeepLearning* &pClassifier, 
			const std::string& prototxt, const std::string& caffemodel, bool is_gpu)
		{
			if (!pClassifier)
				pClassifier = new DeepLearning;

			if (!pClassifier)
			{
				GLOG("ERROR: new pClassifier fail!\n");
				return false;
			}

			if (false == CheckFile(prototxt) || false == CheckFile(caffemodel))
			{
				return false;
			}

			pClassifier->SetBlasThreadNum(s_nNumBlasThread);
			std::vector<int> meanvalue = {104, 117, 123};
			float scale = 0.0078125f;

			if (false == pClassifier->Init(prototxt, meanvalue, caffemodel, is_gpu))
			{
				GLOG("pClassifier->Init error!\n");
				return false;
			}

			pClassifier->SetScale(scale);

			return true;
		}

		ErrorCode BUAnalysis::ProcessResults(BUAnalysisResult& results)
		{
			if (results.nLessionsCount != 0 && results.nGrading == LG1a)
			{
				bool is_lession = false;
				for (int i = 0; i < results.nLessionsCount; i++)
				{
					if (results.pLessionTypes[i] == LESSION
						|| results.pLessionConfidence[i] > 0.5)
					{
						results.nGrading = LG_OTHER;
						is_lession = true;
						break;
					}
				}

				if (is_lession == false)
				{
					results.nLessionsCount = 0;
				}
			}

			if (results.nLessionsCount == 0 && results.nGrading == LG_OTHER)
			{
				results.nGrading = LG1a;
			}

			return SYY_NO_ERROR;
		}

	}
}
