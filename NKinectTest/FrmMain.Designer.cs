using System;

namespace NKinectTest {
    partial class FrmMain {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.imgDisplay = new System.Windows.Forms.PictureBox();
            this.lblDepth = new System.Windows.Forms.Label();
            this.btnExport = new System.Windows.Forms.Button();
            this.cmbImageType = new System.Windows.Forms.ComboBox();
            this.btnCopy = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.imgDisplay)).BeginInit();
            this.SuspendLayout();
            // 
            // imgDisplay
            // 
            this.imgDisplay.BackColor = System.Drawing.Color.Silver;
            this.imgDisplay.Location = new System.Drawing.Point(15, 12);
            this.imgDisplay.Name = "imgDisplay";
            this.imgDisplay.Size = new System.Drawing.Size(640, 480);
            this.imgDisplay.TabIndex = 0;
            this.imgDisplay.TabStop = false;
            this.imgDisplay.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ImgColorMouseMove);
            // 
            // lblDepth
            // 
            this.lblDepth.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDepth.Location = new System.Drawing.Point(16, 518);
            this.lblDepth.Name = "lblDepth";
            this.lblDepth.Size = new System.Drawing.Size(639, 13);
            this.lblDepth.TabIndex = 3;
            this.lblDepth.Text = "DEPTH";
            this.lblDepth.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // btnExport
            // 
            this.btnExport.Location = new System.Drawing.Point(600, 492);
            this.btnExport.Name = "btnExport";
            this.btnExport.Size = new System.Drawing.Size(55, 23);
            this.btnExport.TabIndex = 5;
            this.btnExport.Text = "&Export";
            this.btnExport.UseVisualStyleBackColor = true;
            this.btnExport.Click += new System.EventHandler(this.BtnExportClick);
            // 
            // cmbImageType
            // 
            this.cmbImageType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbImageType.FormattingEnabled = true;
            this.cmbImageType.Items.AddRange(new object[] {
            "Color",
            "Depth",
            "Threshold Depth",
            "Threshold Color"});
            this.cmbImageType.Location = new System.Drawing.Point(275, 495);
            this.cmbImageType.Name = "cmbImageType";
            this.cmbImageType.Size = new System.Drawing.Size(121, 21);
            this.cmbImageType.TabIndex = 7;
            this.cmbImageType.SelectedIndexChanged += new System.EventHandler(this.CmbImageTypeSelectedIndexChanged);
            // 
            // btnCopy
            // 
            this.btnCopy.Location = new System.Drawing.Point(539, 492);
            this.btnCopy.Name = "btnCopy";
            this.btnCopy.Size = new System.Drawing.Size(55, 23);
            this.btnCopy.TabIndex = 14;
            this.btnCopy.Text = "&Copy";
            this.btnCopy.UseVisualStyleBackColor = true;
            this.btnCopy.Click += new System.EventHandler(this.BtnCopyClick);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(671, 539);
            this.Controls.Add(this.btnCopy);
            this.Controls.Add(this.cmbImageType);
            this.Controls.Add(this.btnExport);
            this.Controls.Add(this.lblDepth);
            this.Controls.Add(this.imgDisplay);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "NKinect Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMainFormClosing);
            this.Load += new System.EventHandler(this.FrmMainLoad);
            ((System.ComponentModel.ISupportInitialize)(this.imgDisplay)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox imgDisplay;
        private System.Windows.Forms.Label lblDepth;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.ComboBox cmbImageType;
        private System.Windows.Forms.Button btnCopy;
    }
}

