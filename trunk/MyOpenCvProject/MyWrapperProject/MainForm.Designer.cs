namespace ImageStitcher
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.buttonStitch = new System.Windows.Forms.Button();
            this.groupBoxStitch = new System.Windows.Forms.GroupBox();
            this.labelProgress = new System.Windows.Forms.Label();
            this.buttonBrowse2 = new System.Windows.Forms.Button();
            this.bottonBrowse1 = new System.Windows.Forms.Button();
            this.textBoxImage2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxImage1 = new System.Windows.Forms.TextBox();
            this.labelImage1 = new System.Windows.Forms.Label();
            this.groupBoxDirection = new System.Windows.Forms.GroupBox();
            this.radioButtonVertical = new System.Windows.Forms.RadioButton();
            this.radioButtonHorizontal = new System.Windows.Forms.RadioButton();
            this.groupBoxStitch.SuspendLayout();
            this.groupBoxDirection.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonStitch
            // 
            this.buttonStitch.Location = new System.Drawing.Point(69, 135);
            this.buttonStitch.Name = "buttonStitch";
            this.buttonStitch.Size = new System.Drawing.Size(148, 31);
            this.buttonStitch.TabIndex = 6;
            this.buttonStitch.Text = "Stitch";
            this.buttonStitch.UseVisualStyleBackColor = true;
            this.buttonStitch.Click += new System.EventHandler(this.buttonStitch_Click);
            // 
            // groupBoxStitch
            // 
            this.groupBoxStitch.Controls.Add(this.groupBoxDirection);
            this.groupBoxStitch.Controls.Add(this.labelProgress);
            this.groupBoxStitch.Controls.Add(this.buttonBrowse2);
            this.groupBoxStitch.Controls.Add(this.bottonBrowse1);
            this.groupBoxStitch.Controls.Add(this.textBoxImage2);
            this.groupBoxStitch.Controls.Add(this.label1);
            this.groupBoxStitch.Controls.Add(this.textBoxImage1);
            this.groupBoxStitch.Controls.Add(this.labelImage1);
            this.groupBoxStitch.Controls.Add(this.buttonStitch);
            this.groupBoxStitch.Location = new System.Drawing.Point(12, 11);
            this.groupBoxStitch.Name = "groupBoxStitch";
            this.groupBoxStitch.Size = new System.Drawing.Size(456, 172);
            this.groupBoxStitch.TabIndex = 7;
            this.groupBoxStitch.TabStop = false;
            this.groupBoxStitch.Text = "Stitch:";
            // 
            // labelProgress
            // 
            this.labelProgress.Image = ((System.Drawing.Image)(resources.GetObject("labelProgress.Image")));
            this.labelProgress.Location = new System.Drawing.Point(223, 133);
            this.labelProgress.Name = "labelProgress";
            this.labelProgress.Size = new System.Drawing.Size(42, 36);
            this.labelProgress.TabIndex = 13;
            this.labelProgress.Visible = false;
            // 
            // buttonBrowse2
            // 
            this.buttonBrowse2.Location = new System.Drawing.Point(402, 47);
            this.buttonBrowse2.Name = "buttonBrowse2";
            this.buttonBrowse2.Size = new System.Drawing.Size(41, 23);
            this.buttonBrowse2.TabIndex = 12;
            this.buttonBrowse2.Text = "...";
            this.buttonBrowse2.UseVisualStyleBackColor = true;
            this.buttonBrowse2.Click += new System.EventHandler(this.buttonBrowse2_Click);
            // 
            // bottonBrowse1
            // 
            this.bottonBrowse1.Location = new System.Drawing.Point(402, 16);
            this.bottonBrowse1.Name = "bottonBrowse1";
            this.bottonBrowse1.Size = new System.Drawing.Size(41, 23);
            this.bottonBrowse1.TabIndex = 11;
            this.bottonBrowse1.Text = "...";
            this.bottonBrowse1.UseVisualStyleBackColor = true;
            this.bottonBrowse1.Click += new System.EventHandler(this.bottonBrowse1_Click);
            // 
            // textBoxImage2
            // 
            this.textBoxImage2.Location = new System.Drawing.Point(69, 50);
            this.textBoxImage2.Name = "textBoxImage2";
            this.textBoxImage2.Size = new System.Drawing.Size(328, 20);
            this.textBoxImage2.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Image2:";
            // 
            // textBoxImage1
            // 
            this.textBoxImage1.Location = new System.Drawing.Point(69, 18);
            this.textBoxImage1.Name = "textBoxImage1";
            this.textBoxImage1.Size = new System.Drawing.Size(328, 20);
            this.textBoxImage1.TabIndex = 8;
            // 
            // labelImage1
            // 
            this.labelImage1.AutoSize = true;
            this.labelImage1.Location = new System.Drawing.Point(16, 21);
            this.labelImage1.Name = "labelImage1";
            this.labelImage1.Size = new System.Drawing.Size(45, 13);
            this.labelImage1.TabIndex = 7;
            this.labelImage1.Text = "Image1:";
            // 
            // groupBoxDirection
            // 
            this.groupBoxDirection.Controls.Add(this.radioButtonHorizontal);
            this.groupBoxDirection.Controls.Add(this.radioButtonVertical);
            this.groupBoxDirection.Location = new System.Drawing.Point(69, 78);
            this.groupBoxDirection.Name = "groupBoxDirection";
            this.groupBoxDirection.Size = new System.Drawing.Size(328, 44);
            this.groupBoxDirection.TabIndex = 14;
            this.groupBoxDirection.TabStop = false;
            this.groupBoxDirection.Text = "Direction:";
            // 
            // radioButtonVertical
            // 
            this.radioButtonVertical.AutoSize = true;
            this.radioButtonVertical.Checked = true;
            this.radioButtonVertical.Location = new System.Drawing.Point(60, 18);
            this.radioButtonVertical.Name = "radioButtonVertical";
            this.radioButtonVertical.Size = new System.Drawing.Size(60, 17);
            this.radioButtonVertical.TabIndex = 0;
            this.radioButtonVertical.TabStop = true;
            this.radioButtonVertical.Text = "Vertical";
            this.radioButtonVertical.UseVisualStyleBackColor = true;
            // 
            // radioButtonHorizontal
            // 
            this.radioButtonHorizontal.AutoSize = true;
            this.radioButtonHorizontal.Location = new System.Drawing.Point(240, 17);
            this.radioButtonHorizontal.Name = "radioButtonHorizontal";
            this.radioButtonHorizontal.Size = new System.Drawing.Size(72, 17);
            this.radioButtonHorizontal.TabIndex = 1;
            this.radioButtonHorizontal.TabStop = true;
            this.radioButtonHorizontal.Text = "Horizontal";
            this.radioButtonHorizontal.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(476, 189);
            this.Controls.Add(this.groupBoxStitch);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.groupBoxStitch.ResumeLayout(false);
            this.groupBoxStitch.PerformLayout();
            this.groupBoxDirection.ResumeLayout(false);
            this.groupBoxDirection.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonStitch;
        private System.Windows.Forms.GroupBox groupBoxStitch;
        private System.Windows.Forms.Button buttonBrowse2;
        private System.Windows.Forms.Button bottonBrowse1;
        private System.Windows.Forms.TextBox textBoxImage2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxImage1;
        private System.Windows.Forms.Label labelImage1;
        private System.Windows.Forms.Label labelProgress;
        private System.Windows.Forms.GroupBox groupBoxDirection;
        private System.Windows.Forms.RadioButton radioButtonHorizontal;
        private System.Windows.Forms.RadioButton radioButtonVertical;


    }
}