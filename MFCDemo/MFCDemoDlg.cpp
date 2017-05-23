
// MFCDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"
#include "afxdialogex.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace SYY;
using namespace SYY::MedicalAnalysis;


// CMFCDemoDlg �Ի���

CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FOLDER, &CMFCDemoDlg::OnBnClickedButtonOpenFolder)
	ON_BN_CLICKED(IDC_BUTTON_ANALYSIS, &CMFCDemoDlg::OnBnClickedButtonAnalysis)
	ON_LBN_SELCHANGE(IDC_LIST_FILELIST, &CMFCDemoDlg::OnSelchangeListFilelist)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMFCDemoDlg ��Ϣ�������

BOOL CMFCDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	GetInitImageRect();

	auto listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	listbox->SetHorizontalExtent(1000);

	ErrorCode code;
	char msg[256];
	if (SYY_NO_ERROR != (code = InitSDK()))
	{
		sprintf_s(msg, "��ʼ���㷨ʧ��! ������Ϊ%d", code);
		MessageBox(msg);
	}
	else {
		if (SYY_NO_ERROR != (code = InitBUAnalysis(m_hHandle, BUAnalysisMode::Crop_V1))) {
			sprintf_s(msg, "��ʼ��B�������㷨ʧ��! ������Ϊ%d", code);
			MessageBox(msg);
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CMFCDemoDlg::OnLbnSelchangeList1()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CMFCDemoDlg::OnBnClickedButtonOpenFolder()
{
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·��

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "��ѡ����Ҫ�򿪵�Ŀ¼��";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(lp, szPath);

	CString folderRootPath = szPath;
	
	std::vector<std::string> files;
	scanFilesUseRecursive(folderRootPath.GetString(), files);

	ResetAll();

	for (auto file : files)
	{
		int idx = file.find_last_of('.');
		if (idx == -1)
			continue;

		auto ext = file.substr(idx + 1);
		if (ext != "jpg" && ext != "bmp" && ext != "png")
			continue;

		m_vFiles.push_back(file);
	}

	if (m_vFiles.size() == 0)
	{
		MessageBox("��Ŀ¼��û��ͼƬ�ļ���");
		return;
	}

	RefreshFileListBox();
}


void CMFCDemoDlg::OnBnClickedButtonAnalysis()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_mCurImg.empty())
	{
		MessageBox("����ѡ��ͼƬ!");
		return;
	}

	auto img = m_mCurImg.clone();

	BUAnalysisResult result;
	ErrorCode code;
	bool bIsCrop = true;
	if (SYY_NO_ERROR != (code = 
		ExecuteBUAnalysis(m_hHandle, (char*)img.data, img.cols, img.rows, &result)))
	{
		char msg[256];
		sprintf_s(msg, "b�������㷨ʧ�ܣ� ������Ϊ�� %d", code);
		MessageBox(msg);
		return;
	}

	cv::Rect cropRect(result.rCropRect.x, result.rCropRect.y, result.rCropRect.w, result.rCropRect.h);
	cv::rectangle(img, cropRect, cv::Scalar(255, 255, 255), 4);

	for (int i = 0; i < result.nLessionsCount; i++)
	{
		auto r = result.pLessionRects[i];
		cv::Rect rect(r.x, r.y, r.w, r.h);
		cv::rectangle(img, rect, cv::Scalar(255, 255, 255), 3);
	}

	ShowImg(img);
}


void CMFCDemoDlg::OnSelchangeListFilelist()
{
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	m_nCurFileIdx = listbox->GetCurSel();

	ShowImgFromFileIdx(m_nCurFileIdx);
}

void CMFCDemoDlg::ResetAll()
{
	m_vFiles.clear();
}

void CMFCDemoDlg::RefreshFileListBox()
{
	CListBox* listbox_filepath = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);

	listbox_filepath->ResetContent();
	int nFilesLen = m_vFiles.size();
	for (int i = 0; i < nFilesLen; i++)
	{
		CString file = CString(m_vFiles[i].c_str());

		CString line;
		line.Format(" (%d) %s", i + 1, file);

		listbox_filepath->InsertString(i, line);
	}
}

void CMFCDemoDlg::GetInitImageRect()
{
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); //���pictrue�ؼ����ڵľ��   
	CRect rect, rect_windows;
	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ������� 
	rect_windows = rect;
	ScreenToClient(&rect_windows);
	m_rImageInit = rect_windows;
	m_mBgMat = cv::Mat(rect.Height(), rect.Width(), CV_8UC3, cv::Scalar(255, 255, 255));
}


bool CMFCDemoDlg::ShowImg(const cv::Mat& src)
{
	cv::Mat resize_img;

	CWnd *pWnd = GetDlgItem(IDC_IMAGE); //���pictrue�ؼ����ڵľ��   
	pWnd->UpdateWindow();

	pWnd->SetWindowPos(NULL, m_rImageInit.left, m_rImageInit.top, m_rImageInit.Width(), m_rImageInit.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	CRect rect(0, 0, src.cols, src.rows);
	pWnd->GetClientRect(&rect);//���pictrue�ؼ����ڵľ�������   
	AdjustInputMatSizeAndRect(src, resize_img, rect);

	m_resizeImgSize.width = rect.Width();
	m_resizeImgSize.height = rect.Height();

	CImage c_mat;
	MatToCImage(src, c_mat);

	CDC *pDC = pWnd->GetDC();//���pictrue�ؼ���DC   

	c_mat.Draw(pDC->m_hDC, rect); //��ͼƬ����Picture�ؼ���ʾ�ľ�������   

	ReleaseDC(pDC);

	return true;
}

bool CMFCDemoDlg::AdjustInputMatSizeAndRect(const cv::Mat& src, cv::Mat&dst, CRect& rect)
{
	double m_rate = double(src.cols) / double(src.rows);
	double cr_rate = double(rect.Width()) / double(rect.Height());
	int width = 0, height = 0;
	if (cr_rate >= m_rate)
	{
		height = rect.Height();
		width = height*m_rate;
	}
	else
	{
		width = rect.Width();
		height = width / m_rate;
	}

	rect.bottom = rect.top + height;
	rect.right = rect.left + width;

	resize(src, dst, cv::Size(width, height), cv::INTER_LANCZOS4);

	return true;
}

void CMFCDemoDlg::MatToCImage(const cv::Mat& mat, CImage& cImage)
{
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	cImage.Destroy();//��һ���Ƿ�ֹ�ظ���������ڴ�����  
	cImage.Create(width, height, 8 * channels);

	const uchar* ps;
	uchar* pimg = (uchar*)cImage.GetBits(); //��ȡCImage�����ش�������ָ��  
	int step = cImage.GetPitch();//ÿ�е��ֽ���,ע���������ֵ�����и�  

	// �����1��ͨ����ͼ��(�Ҷ�ͼ��) DIB��ʽ����Ҫ�Ե�ɫ������    
	// CImage�������˵�ɫ�壬����Ҫ�������и�ֵ��  
	if (1 == channels)
	{
		RGBQUAD* ColorTable;
		int MaxColors = 256;
		//�������ͨ��CI.GetMaxColorTableEntries()�õ���С(�������CI.Load����ͼ��Ļ�)    
		ColorTable = new RGBQUAD[MaxColors];
		cImage.GetColorTable(0, MaxColors, ColorTable);//������ȡ��ָ��    
		for (int i = 0; i<MaxColors; i++)
		{
			ColorTable[i].rgbBlue = (BYTE)i;
			//BYTE��ucharһ���£���MFC�ж�����    
			ColorTable[i].rgbGreen = (BYTE)i;
			ColorTable[i].rgbRed = (BYTE)i;
		}
		cImage.SetColorTable(0, MaxColors, ColorTable);
		delete[]ColorTable;
	}


	for (int i = 0; i < height; i++)
	{
		ps = mat.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (1 == channels)
			{
				*(pimg + i*step + j) = ps[j];
				//*(pimg + i*step + j) = 105;  
			}
			else if (3 == channels)
			{
				*(pimg + i*step + j * 3) = ps[j * 3];
				*(pimg + i*step + j * 3 + 1) = ps[j * 3 + 1];
				*(pimg + i*step + j * 3 + 2) = ps[j * 3 + 2];
			}
		}
	}
	//string str = CString2StdString(_T("C:\\sample1020.bmp"));  
	//imwrite(str,mat);  
	//��仰������������cimage��û�б���ֵ  
	//cImage.Save(_T("C:\\sample1024.bmp"));  
	//return;
}

void CMFCDemoDlg::ShowImgFromFileIdx(int idx)
{
	if (idx >= m_vFiles.size())
		return;

	m_nCurFileIdx = idx;
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST_FILELIST);
	listbox->SetCurSel(m_nCurFileIdx);
	listbox->UpdateData();

	auto file = m_vFiles[idx];
	auto img = cv::imread(file);
	if (img.empty())
	{
		CString line;
		line.Format("���ܶ�ȡͼƬ�� %s", file.c_str());
		MessageBox(line);
		return;
	}

	m_mOriImg = img;

	ShowImg(m_mBgMat);

	Redraw();
}

void CMFCDemoDlg::Redraw()
{
	if (m_mOriImg.empty())
		return;
	
	auto img = m_mOriImg.clone();
	m_mCurImg = img;

	ShowImg(img);
}


void CMFCDemoDlg::ScreenPt2ImgPt(CPoint snPt, cv::Point& imgPt)
{
	CRect Prect1;          //����ͼƬ�ľ���

	GetDlgItem(IDC_IMAGE)->GetWindowRect(&Prect1);    //�õ�ͼƬ�ľ�//�δ�С
	ScreenToClient(&Prect1);   //��ͼƬ��ľ��Ծ��δ�С

	CPoint point = snPt;

	//�ж��Ƿ���ͼƬ���ڣ���������ͼƬ���ڵĵ��
	//if (point.x<Prect1.left || point.x>Prect1.right || point.y<Prect1.top || point.y>Prect1.bottom) return;
	imgPt.x = snPt.x - Prect1.left;
	imgPt.y = snPt.y - Prect1.top;

	imgPt.x = std::max(0, std::min(imgPt.x, int(Prect1.right)));
	imgPt.y = std::max(0, std::min(imgPt.y, int(Prect1.bottom)));
}

void CMFCDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bIsLButtonDown = true;

	if (false == m_mCurImg.empty() && m_bIsLButtonDown)
	{
		cv::Point pt;
		ScreenPt2ImgPt(point, pt);

		auto oriSize = m_mOriImg.size();
		float rx = (float)oriSize.width / (float)m_resizeImgSize.width;
		float ry = (float)oriSize.height / (float)m_resizeImgSize.height;

		pt.x *= rx;
		pt.y *= ry;

		cv::circle(m_mCurImg, pt, 5, cv::Scalar(0, 0, 0), 5);

		ShowImg(m_mCurImg);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bIsLButtonDown = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (false == m_mCurImg.empty() && m_bIsLButtonDown)
	{
		cv::Point pt;
		ScreenPt2ImgPt(point, pt);

		auto oriSize = m_mOriImg.size();
		float rx = (float)oriSize.width / (float)m_resizeImgSize.width;
		float ry = (float)oriSize.height / (float)m_resizeImgSize.height;

		pt.x *= rx;
		pt.y *= ry;

		cv::circle(m_mCurImg, pt, 5, cv::Scalar(0, 0, 0), 5);

		ShowImg(m_mCurImg);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
