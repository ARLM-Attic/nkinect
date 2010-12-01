﻿using System;

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
            this.imgColor = new System.Windows.Forms.PictureBox();
            this.lblColor = new System.Windows.Forms.Label();
            this.lblDepth = new System.Windows.Forms.Label();
            this.trkPosition = new System.Windows.Forms.TrackBar();
            this.btnExport = new System.Windows.Forms.Button();
            this.lblAccelerometer = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.imgColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).BeginInit();
            this.SuspendLayout();
            // 
            // imgColor
            // 
            this.imgColor.BackColor = System.Drawing.Color.Silver;
            this.imgColor.Location = new System.Drawing.Point(15, 12);
            this.imgColor.Name = "imgColor";
            this.imgColor.Size = new System.Drawing.Size(640, 480);
            this.imgColor.TabIndex = 0;
            this.imgColor.TabStop = false;
            this.imgColor.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ImgColorMouseMove);
            // 
            // lblColor
            // 
            this.lblColor.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblColor.Location = new System.Drawing.Point(15, 495);
            this.lblColor.Name = "lblColor";
            this.lblColor.Size = new System.Drawing.Size(640, 13);
            this.lblColor.TabIndex = 2;
            this.lblColor.Text = "COLOR";
            this.lblColor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
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
            // trkPosition
            // 
            this.trkPosition.Location = new System.Drawing.Point(15, 533);
            this.trkPosition.Maximum = 8000;
            this.trkPosition.Minimum = -8000;
            this.trkPosition.Name = "trkPosition";
            this.trkPosition.Size = new System.Drawing.Size(640, 45);
            this.trkPosition.TabIndex = 4;
            this.trkPosition.ValueChanged += new System.EventHandler(this.TrkPositionValueChanged);
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
            // lblAccelerometer
            // 
            this.lblAccelerometer.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblAccelerometer.Location = new System.Drawing.Point(12, 592);
            this.lblAccelerometer.Name = "lblAccelerometer";
            this.lblAccelerometer.Size = new System.Drawing.Size(640, 13);
            this.lblAccelerometer.TabIndex = 6;
            this.lblAccelerometer.Text = "ACCELEROMETER";
            this.lblAccelerometer.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(671, 614);
            this.Controls.Add(this.lblAccelerometer);
            this.Controls.Add(this.btnExport);
            this.Controls.Add(this.trkPosition);
            this.Controls.Add(this.lblDepth);
            this.Controls.Add(this.lblColor);
            this.Controls.Add(this.imgColor);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Kinect Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMainFormClosing);
            this.Load += new System.EventHandler(this.FrmMainLoad);
            ((System.ComponentModel.ISupportInitialize)(this.imgColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox imgColor;
        private System.Windows.Forms.Label lblColor;
        private System.Windows.Forms.Label lblDepth;
        private System.Windows.Forms.TrackBar trkPosition;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Label lblAccelerometer;
    }
}

