using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ImageStitcher
{
    public partial class MainForm : Form
    {
        String image1Path="";
        String image2Path="";
        public MainForm()
        {
            InitializeComponent();
        }

        private void bottonBrowse1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            DialogResult result= openFileDialog.ShowDialog(this);
            if(result==DialogResult.OK){
                this.image1Path = openFileDialog.FileName;
                this.textBoxImage1.Text = this.image1Path;
            }
        }

        private void buttonBrowse2_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            DialogResult result = openFileDialog.ShowDialog(this);
            if (result == DialogResult.OK)
            {
                this.image2Path = openFileDialog.FileName;
                this.textBoxImage2.Text = this.image2Path;
            }
        }

        private void buttonStitch_Click(object sender, EventArgs e)
        {
            if (this.image1Path.Length == 0 || this.image2Path.Length == 0)
            {
                MessageBox.Show("Pleas select image file");
                return;
            }
            StitcherWrapper.Stitch(this.image1Path, this.image2Path);
        }
    }
}
