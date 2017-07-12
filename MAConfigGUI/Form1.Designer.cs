namespace MAConfigGUI
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label_crop_v2_prototxt = new System.Windows.Forms.Label();
            this.button_crop_v2_prototxt = new System.Windows.Forms.Button();
            this.button_crop_v2_caffemodel = new System.Windows.Forms.Button();
            this.label_crop_v2_caffemodel = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button_analysis_prototxt = new System.Windows.Forms.Button();
            this.label_analysis_prototxt = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label_analysis_caffemodel = new System.Windows.Forms.Label();
            this.button_analysis_caffemodel = new System.Windows.Forms.Button();
            this.button_crop_v2_config = new System.Windows.Forms.Button();
            this.label_crop_v2_config = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label_classify_caffemodel = new System.Windows.Forms.Label();
            this.button_classify_caffemodel = new System.Windows.Forms.Button();
            this.label_classify_prototxt = new System.Windows.Forms.Label();
            this.button_classify_prototxt = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label_lessionType_caffemodel = new System.Windows.Forms.Label();
            this.button_lessionType_caffemodel = new System.Windows.Forms.Button();
            this.label_lessionType_prototxt = new System.Windows.Forms.Label();
            this.button_lessionType_prototxt = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.button_load_config = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button_crop_v2_config);
            this.groupBox1.Controls.Add(this.label_crop_v2_config);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.button_crop_v2_caffemodel);
            this.groupBox1.Controls.Add(this.label_crop_v2_caffemodel);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.button_crop_v2_prototxt);
            this.groupBox1.Controls.Add(this.label_crop_v2_prototxt);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(12, 45);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(547, 125);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Crop 参数";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "模型文件prototxt";
            // 
            // label_crop_v2_prototxt
            // 
            this.label_crop_v2_prototxt.AutoSize = true;
            this.label_crop_v2_prototxt.Location = new System.Drawing.Point(137, 22);
            this.label_crop_v2_prototxt.Name = "label_crop_v2_prototxt";
            this.label_crop_v2_prototxt.Size = new System.Drawing.Size(53, 12);
            this.label_crop_v2_prototxt.TabIndex = 1;
            this.label_crop_v2_prototxt.Text = "prototxt";
            // 
            // button_crop_v2_prototxt
            // 
            this.button_crop_v2_prototxt.Location = new System.Drawing.Point(466, 17);
            this.button_crop_v2_prototxt.Name = "button_crop_v2_prototxt";
            this.button_crop_v2_prototxt.Size = new System.Drawing.Size(75, 23);
            this.button_crop_v2_prototxt.TabIndex = 2;
            this.button_crop_v2_prototxt.Text = "设置";
            this.button_crop_v2_prototxt.UseVisualStyleBackColor = true;
            this.button_crop_v2_prototxt.Click += new System.EventHandler(this.button1_Click);
            // 
            // button_crop_v2_caffemodel
            // 
            this.button_crop_v2_caffemodel.Location = new System.Drawing.Point(465, 53);
            this.button_crop_v2_caffemodel.Name = "button_crop_v2_caffemodel";
            this.button_crop_v2_caffemodel.Size = new System.Drawing.Size(75, 23);
            this.button_crop_v2_caffemodel.TabIndex = 5;
            this.button_crop_v2_caffemodel.Text = "设置";
            this.button_crop_v2_caffemodel.UseVisualStyleBackColor = true;
            this.button_crop_v2_caffemodel.Click += new System.EventHandler(this.button2_Click);
            // 
            // label_crop_v2_caffemodel
            // 
            this.label_crop_v2_caffemodel.AutoSize = true;
            this.label_crop_v2_caffemodel.Location = new System.Drawing.Point(136, 58);
            this.label_crop_v2_caffemodel.Name = "label_crop_v2_caffemodel";
            this.label_crop_v2_caffemodel.Size = new System.Drawing.Size(65, 12);
            this.label_crop_v2_caffemodel.TabIndex = 4;
            this.label_crop_v2_caffemodel.Text = "caffemodel";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 58);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(113, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "模型权重caffemodel";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.label_analysis_caffemodel);
            this.groupBox3.Controls.Add(this.button_analysis_caffemodel);
            this.groupBox3.Controls.Add(this.label_analysis_prototxt);
            this.groupBox3.Controls.Add(this.button_analysis_prototxt);
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Location = new System.Drawing.Point(12, 176);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(547, 90);
            this.groupBox3.TabIndex = 1;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Analysis 参数";
            // 
            // button_analysis_prototxt
            // 
            this.button_analysis_prototxt.Location = new System.Drawing.Point(465, 24);
            this.button_analysis_prototxt.Name = "button_analysis_prototxt";
            this.button_analysis_prototxt.Size = new System.Drawing.Size(75, 23);
            this.button_analysis_prototxt.TabIndex = 7;
            this.button_analysis_prototxt.Text = "设置";
            this.button_analysis_prototxt.UseVisualStyleBackColor = true;
            this.button_analysis_prototxt.Click += new System.EventHandler(this.button_analysis_prototxt_Click);
            // 
            // label_analysis_prototxt
            // 
            this.label_analysis_prototxt.AutoSize = true;
            this.label_analysis_prototxt.Location = new System.Drawing.Point(137, 28);
            this.label_analysis_prototxt.Name = "label_analysis_prototxt";
            this.label_analysis_prototxt.Size = new System.Drawing.Size(53, 12);
            this.label_analysis_prototxt.TabIndex = 8;
            this.label_analysis_prototxt.Text = "prototxt";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(19, 28);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(101, 12);
            this.label8.TabIndex = 7;
            this.label8.Text = "模型文件prototxt";
            // 
            // label_analysis_caffemodel
            // 
            this.label_analysis_caffemodel.AutoSize = true;
            this.label_analysis_caffemodel.Location = new System.Drawing.Point(138, 63);
            this.label_analysis_caffemodel.Name = "label_analysis_caffemodel";
            this.label_analysis_caffemodel.Size = new System.Drawing.Size(65, 12);
            this.label_analysis_caffemodel.TabIndex = 11;
            this.label_analysis_caffemodel.Text = "caffemodel";
            // 
            // button_analysis_caffemodel
            // 
            this.button_analysis_caffemodel.Location = new System.Drawing.Point(466, 58);
            this.button_analysis_caffemodel.Name = "button_analysis_caffemodel";
            this.button_analysis_caffemodel.Size = new System.Drawing.Size(75, 23);
            this.button_analysis_caffemodel.TabIndex = 9;
            this.button_analysis_caffemodel.Text = "设置";
            this.button_analysis_caffemodel.UseVisualStyleBackColor = true;
            this.button_analysis_caffemodel.Click += new System.EventHandler(this.button_analysis_caffemodel_Click);
            // 
            // button_crop_v2_config
            // 
            this.button_crop_v2_config.Location = new System.Drawing.Point(464, 89);
            this.button_crop_v2_config.Name = "button_crop_v2_config";
            this.button_crop_v2_config.Size = new System.Drawing.Size(75, 23);
            this.button_crop_v2_config.TabIndex = 8;
            this.button_crop_v2_config.Text = "设置";
            this.button_crop_v2_config.UseVisualStyleBackColor = true;
            this.button_crop_v2_config.Click += new System.EventHandler(this.button_crop_v2_config_Click);
            // 
            // label_crop_v2_config
            // 
            this.label_crop_v2_config.AutoSize = true;
            this.label_crop_v2_config.Location = new System.Drawing.Point(135, 94);
            this.label_crop_v2_config.Name = "label_crop_v2_config";
            this.label_crop_v2_config.Size = new System.Drawing.Size(41, 12);
            this.label_crop_v2_config.TabIndex = 7;
            this.label_crop_v2_config.Text = "config";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 94);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "模型配置config";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(19, 63);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(113, 12);
            this.label5.TabIndex = 9;
            this.label5.Text = "模型权重caffemodel";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 12);
            this.label2.TabIndex = 9;
            this.label2.Text = "模型权重caffemodel";
            // 
            // label_classify_caffemodel
            // 
            this.label_classify_caffemodel.AutoSize = true;
            this.label_classify_caffemodel.Location = new System.Drawing.Point(138, 63);
            this.label_classify_caffemodel.Name = "label_classify_caffemodel";
            this.label_classify_caffemodel.Size = new System.Drawing.Size(65, 12);
            this.label_classify_caffemodel.TabIndex = 11;
            this.label_classify_caffemodel.Text = "caffemodel";
            // 
            // button_classify_caffemodel
            // 
            this.button_classify_caffemodel.Location = new System.Drawing.Point(466, 58);
            this.button_classify_caffemodel.Name = "button_classify_caffemodel";
            this.button_classify_caffemodel.Size = new System.Drawing.Size(75, 23);
            this.button_classify_caffemodel.TabIndex = 9;
            this.button_classify_caffemodel.Text = "设置";
            this.button_classify_caffemodel.UseVisualStyleBackColor = true;
            this.button_classify_caffemodel.Click += new System.EventHandler(this.button_classify_caffemodel_Click);
            // 
            // label_classify_prototxt
            // 
            this.label_classify_prototxt.AutoSize = true;
            this.label_classify_prototxt.Location = new System.Drawing.Point(137, 28);
            this.label_classify_prototxt.Name = "label_classify_prototxt";
            this.label_classify_prototxt.Size = new System.Drawing.Size(53, 12);
            this.label_classify_prototxt.TabIndex = 8;
            this.label_classify_prototxt.Text = "prototxt";
            // 
            // button_classify_prototxt
            // 
            this.button_classify_prototxt.Location = new System.Drawing.Point(465, 24);
            this.button_classify_prototxt.Name = "button_classify_prototxt";
            this.button_classify_prototxt.Size = new System.Drawing.Size(75, 23);
            this.button_classify_prototxt.TabIndex = 7;
            this.button_classify_prototxt.Text = "设置";
            this.button_classify_prototxt.UseVisualStyleBackColor = true;
            this.button_classify_prototxt.Click += new System.EventHandler(this.button_classify_prototxt_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(19, 28);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(101, 12);
            this.label10.TabIndex = 7;
            this.label10.Text = "模型文件prototxt";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.label_classify_caffemodel);
            this.groupBox2.Controls.Add(this.button_classify_caffemodel);
            this.groupBox2.Controls.Add(this.label_classify_prototxt);
            this.groupBox2.Controls.Add(this.button_classify_prototxt);
            this.groupBox2.Controls.Add(this.label10);
            this.groupBox2.Location = new System.Drawing.Point(12, 272);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(547, 90);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "病理分级 参数";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.label_lessionType_caffemodel);
            this.groupBox4.Controls.Add(this.button_lessionType_caffemodel);
            this.groupBox4.Controls.Add(this.label_lessionType_prototxt);
            this.groupBox4.Controls.Add(this.button_lessionType_prototxt);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Location = new System.Drawing.Point(12, 368);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(547, 90);
            this.groupBox4.TabIndex = 13;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "病灶分级 参数";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(19, 57);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(113, 12);
            this.label6.TabIndex = 9;
            this.label6.Text = "模型权重caffemodel";
            // 
            // label_lessionType_caffemodel
            // 
            this.label_lessionType_caffemodel.AutoSize = true;
            this.label_lessionType_caffemodel.Location = new System.Drawing.Point(138, 57);
            this.label_lessionType_caffemodel.Name = "label_lessionType_caffemodel";
            this.label_lessionType_caffemodel.Size = new System.Drawing.Size(65, 12);
            this.label_lessionType_caffemodel.TabIndex = 11;
            this.label_lessionType_caffemodel.Text = "caffemodel";
            // 
            // button_lessionType_caffemodel
            // 
            this.button_lessionType_caffemodel.Location = new System.Drawing.Point(466, 52);
            this.button_lessionType_caffemodel.Name = "button_lessionType_caffemodel";
            this.button_lessionType_caffemodel.Size = new System.Drawing.Size(75, 23);
            this.button_lessionType_caffemodel.TabIndex = 9;
            this.button_lessionType_caffemodel.Text = "设置";
            this.button_lessionType_caffemodel.UseVisualStyleBackColor = true;
            this.button_lessionType_caffemodel.Click += new System.EventHandler(this.button_lessionType_caffemodel_Click);
            // 
            // label_lessionType_prototxt
            // 
            this.label_lessionType_prototxt.AutoSize = true;
            this.label_lessionType_prototxt.Location = new System.Drawing.Point(137, 22);
            this.label_lessionType_prototxt.Name = "label_lessionType_prototxt";
            this.label_lessionType_prototxt.Size = new System.Drawing.Size(53, 12);
            this.label_lessionType_prototxt.TabIndex = 8;
            this.label_lessionType_prototxt.Text = "prototxt";
            // 
            // button_lessionType_prototxt
            // 
            this.button_lessionType_prototxt.Location = new System.Drawing.Point(465, 18);
            this.button_lessionType_prototxt.Name = "button_lessionType_prototxt";
            this.button_lessionType_prototxt.Size = new System.Drawing.Size(75, 23);
            this.button_lessionType_prototxt.TabIndex = 7;
            this.button_lessionType_prototxt.Text = "设置";
            this.button_lessionType_prototxt.UseVisualStyleBackColor = true;
            this.button_lessionType_prototxt.Click += new System.EventHandler(this.button_lessionType_prototxt_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(19, 22);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(101, 12);
            this.label11.TabIndex = 7;
            this.label11.Text = "模型文件prototxt";
            // 
            // button_load_config
            // 
            this.button_load_config.Location = new System.Drawing.Point(32, 12);
            this.button_load_config.Name = "button_load_config";
            this.button_load_config.Size = new System.Drawing.Size(86, 23);
            this.button_load_config.TabIndex = 14;
            this.button_load_config.Text = "加载配置文件";
            this.button_load_config.UseVisualStyleBackColor = true;
            this.button_load_config.Click += new System.EventHandler(this.button_load_config_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(576, 470);
            this.Controls.Add(this.button_load_config);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button_crop_v2_caffemodel;
        private System.Windows.Forms.Label label_crop_v2_caffemodel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button_crop_v2_prototxt;
        private System.Windows.Forms.Label label_crop_v2_prototxt;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button_crop_v2_config;
        private System.Windows.Forms.Label label_crop_v2_config;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label_analysis_caffemodel;
        private System.Windows.Forms.Button button_analysis_caffemodel;
        private System.Windows.Forms.Label label_analysis_prototxt;
        private System.Windows.Forms.Button button_analysis_prototxt;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label_classify_caffemodel;
        private System.Windows.Forms.Button button_classify_caffemodel;
        private System.Windows.Forms.Label label_classify_prototxt;
        private System.Windows.Forms.Button button_classify_prototxt;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label_lessionType_caffemodel;
        private System.Windows.Forms.Button button_lessionType_caffemodel;
        private System.Windows.Forms.Label label_lessionType_prototxt;
        private System.Windows.Forms.Button button_lessionType_prototxt;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button button_load_config;
    }
}

