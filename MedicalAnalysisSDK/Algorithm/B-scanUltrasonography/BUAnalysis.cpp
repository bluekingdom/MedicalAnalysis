#include "BUAnalysis.h"
#include "Algorithm/Common/glog.h"

namespace SYY {
	namespace MedicalAnalysis {

		ErrorCode BUAnalysis::Analysis(const cv::Mat& srcImg, BUAnalysisResult& result)
		{
			GLOG("BUAnalysis::Analysis info: enter!\n");
			if (srcImg.empty())
			{
				GLOG("BUAnalysis::Analysis error: srcImg is empty!\n");
				return SYY_SYS_ERROR;
			}

			cv::Rect validRect;
			CropValidRegion(srcImg, validRect);

			auto drawing = srcImg.clone();
			cv::rectangle(drawing, validRect, cv::Scalar(255, 255, 255));
			cv::imshow("drawing", drawing);
			cv::waitKey();

			GLOG("BUAnalysis::Analysis info: exit!\n");
			return SYY_NO_ERROR;
		}

		ErrorCode BUAnalysis::Init()
		{

			return SYY_NO_ERROR;
		}

		ErrorCode BUAnalysis::Release()
		{

			return SYY_NO_ERROR;
		}

		bool BUAnalysis::CropValidRegion(const cv::Mat& srcImg, cv::Rect& validRegion)
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

			cv::findContours(srcImg.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
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
			float radio;

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

	}
}