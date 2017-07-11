using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using SYY;

namespace WindowsFormsAppTest
{
    public partial class Form1 : Form
    {
        ErrorCode error_code = ErrorCode.SYY_NO_ERROR;
        ulong hHandle = 0;
        Bitmap m_Bitmap = null;
        string m_sImageFile = "";

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_sImageFile == "" || false == File.Exists(m_sImageFile))
            {
                MessageBox.Show("文件路径有误： " + m_sImageFile);
                return;
            }

            if (m_Bitmap == null)
            {
                MessageBox.Show("没加载bitmap!");
                return;
            }

            string path = System.Environment.CurrentDirectory;

            CImage image = new CImage();

            if (false == SDK.CvtBitmap2CImage(m_Bitmap, ref image))
            {
                MessageBox.Show("CvtBitmap2CImage出错！");
                return;
            }

            BUAnalysisResult result = new BUAnalysisResult();
            error_code = SDK.ExecuteBUAnalysisFromFile(hHandle, image, ref result);
            if (error_code != ErrorCode.SYY_NO_ERROR)
            {
                MessageBox.Show("运行分析出错，请查看log文件。");
                return;
            }

            error_code = SDK.DrawResult2Image(ref image, result);
            if (error_code != ErrorCode.SYY_NO_ERROR)
            {
                MessageBox.Show("运行转换出错，请查看log文件。");
                return;
            }

            Bitmap cvt_bitmap = new Bitmap(image.nWidth, image.nHeight, image.nChannels * image.nWidth, 
                System.Drawing.Imaging.PixelFormat.Format24bppRgb, image.pData);

            ShowImage(cvt_bitmap);
        }

        private void ShowImage(Bitmap bitmap)
        {
            PictureBox pb = (PictureBox)this.Controls.Find("pictureBox1", true)[0];
            pb.Image = bitmap;
            pb.SizeMode = PictureBoxSizeMode.StretchImage;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // 初始化sdk
            error_code = SDK.InitSDK();
            if (error_code != ErrorCode.SYY_NO_ERROR)
            {
                MessageBox.Show("初始化sdk出错，请查看log文件。");
            }
            MessageBox.Show("初始化sdk。");
        }
        private void button3_Click(object sender, EventArgs e)
        {
            // 加载模型
            error_code = SDK.InitBUAnalysis(ref hHandle);
            if (error_code != ErrorCode.SYY_NO_ERROR)
            {
                MessageBox.Show("初始化模型出错，请查看log文件。");
            }
            MessageBox.Show("初始化模型.");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            // 释放模型
            error_code = SDK.ReleaseBUAnalysis(ref hHandle);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            // 释放sdk
            error_code = SDK.ReleaseSDK();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "C://";
            openFileDialog.Filter = "JPG|*.jpg|PNG|*.png|BMP|*.bmp|其他图片格式|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                //string filePath = "D:\\blue\\data\\乳腺癌图片\\5类\\潘光敏\\1.2.826.0.1.3680043.2.461.8921672.1968559331.jpg";
                m_sImageFile = openFileDialog.FileName;
                m_Bitmap = Bitmap.FromFile(m_sImageFile) as Bitmap;
                ShowImage(m_Bitmap);
            }
        }
    }
}
