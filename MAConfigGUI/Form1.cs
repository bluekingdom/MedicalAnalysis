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


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {

        }

        private void button_crop_v2_config_Click(object sender, EventArgs e)
        {

        }

        private void button_analysis_prototxt_Click(object sender, EventArgs e)
        {

        }

        private void button_analysis_caffemodel_Click(object sender, EventArgs e)
        {

        }

        private void button_classify_prototxt_Click(object sender, EventArgs e)
        {

        }

        private void button_classify_caffemodel_Click(object sender, EventArgs e)
        {

        }

        private void button_lessionType_prototxt_Click(object sender, EventArgs e)
        {

        }

        private void button_lessionType_caffemodel_Click(object sender, EventArgs e)
        {

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
                    m_cXmlReader.Load(openFileDialog.FileName);
                    XmlNode crop_v2_node = m_cXmlReader["config"]["crop_v2"];
                    string crop_v2_prototxt = crop_v2_node["prototxt"].InnerXml;

                    (this.Controls.Find("label_crop_v2_prototxt", true)[0] as Label).Text = crop_v2_prototxt;
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }
    }
}
