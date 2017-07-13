using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace MAConfigGUI
{
    public partial class Form1 : Form
    {
        public XmlDocument m_cXmlReader;
        public string m_sXmlFilaPath;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private string GetSelectFile(string filter = "配置文件|*.cfg")
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "C://";
            openFileDialog.Filter = filter;
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                return openFileDialog.FileName;
            }

            return "";
        }
        private void SetTextFromXml(string first_node, string secord_node, string text_name)
        {
            try
            {
                XmlNode crop_v2_node = m_cXmlReader["config"][first_node];
                string crop_v2_prototxt = crop_v2_node[secord_node].InnerXml;
                (this.Controls.Find(text_name, true)[0] as Label).Text = crop_v2_prototxt;
            }
            catch(Exception e)
            {
                MessageBox.Show("SetTextFromXml" + first_node + " " + secord_node + " " + text_name + " " + e.Message);
            }

        }

        private void SetXml(string first_node, string secord_node, string text_name)
        {
            try 
            {
                string filter = secord_node + "|*." + secord_node;
                string txt = GetSelectFile(filter);
                txt = txt.Substring(txt.LastIndexOf("\\") + 1);
                XmlNode crop_v2_node = m_cXmlReader["config"][first_node];
                crop_v2_node[secord_node].InnerXml = txt;
                (this.Controls.Find(text_name, true)[0] as Label).Text = txt;
                m_cXmlReader.Save(m_sXmlFilaPath);
            }
            catch(Exception e)
            {
                MessageBox.Show("SetXml" + first_node + " " + secord_node + " " + text_name + " " + e.Message);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            SetXml("crop_v2", "prototxt", "label_crop_v2_prototxt");
        }
        private void button2_Click(object sender, EventArgs e)
        {
            SetXml("crop_v2", "caffemodel", "label_crop_v2_caffemodel");
        }

        private void button_crop_v2_config_Click(object sender, EventArgs e)
        {
            SetXml("crop_v2", "config", "label_crop_v2_config");
        }

        private void button_analysis_prototxt_Click(object sender, EventArgs e)
        {
            SetXml("analysis_more", "prototxt", "label_analysis_prototxt");
        }

        private void button_analysis_caffemodel_Click(object sender, EventArgs e)
        {
            SetXml("analysis_more", "caffemodel", "label_analysis_caffemodel");
        }

        private void button_classify_prototxt_Click(object sender, EventArgs e)
        {
            SetXml("grading_classify", "prototxt", "label_classify_prototxt");
        }

        private void button_classify_caffemodel_Click(object sender, EventArgs e)
        {
            SetXml("grading_classify", "caffemodel", "label_classify_caffemodel");
        }

        private void button_lessionType_prototxt_Click(object sender, EventArgs e)
        {
            SetXml("lession_type", "prototxt", "label_lessionType_prototxt");
        }

        private void button_lessionType_caffemodel_Click(object sender, EventArgs e)
        {
            SetXml("lession_type", "caffemodel", "label_lessionType_caffemodel");
        }

        private void button_load_config_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "C://";
            openFileDialog.Filter = "配置文件|*.cfg";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    m_cXmlReader = new XmlDocument();
                    m_sXmlFilaPath = openFileDialog.FileName;
                    m_cXmlReader.Load(m_sXmlFilaPath);
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                SetTextFromXml("crop_v2", "prototxt", "label_crop_v2_prototxt");
                SetTextFromXml("crop_v2", "caffemodel", "label_crop_v2_caffemodel");
                SetTextFromXml("crop_v2", "config", "label_crop_v2_config");

                SetTextFromXml("analysis_more", "prototxt", "label_analysis_prototxt");
                SetTextFromXml("analysis_more", "caffemodel", "label_analysis_caffemodel");

                SetTextFromXml("grading_classify", "prototxt", "label_classify_prototxt");
                SetTextFromXml("grading_classify", "caffemodel", "label_classify_caffemodel");

                SetTextFromXml("lession_type", "prototxt", "label_lessionType_prototxt");
                SetTextFromXml("lession_type", "caffemodel", "label_lessionType_caffemodel");
            }
        }
    }
}
