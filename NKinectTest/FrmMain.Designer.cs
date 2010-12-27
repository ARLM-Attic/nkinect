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
            this.trkPosition = new System.Windows.Forms.TrackBar();
            this.btnExport = new System.Windows.Forms.Button();
            this.lblAccelerometer = new System.Windows.Forms.Label();
            this.cmbImageType = new System.Windows.Forms.ComboBox();
            this.trkMinDistance = new System.Windows.Forms.TrackBar();
            this.trkMaxDistance = new System.Windows.Forms.TrackBar();
            this.cmbUnits = new System.Windows.Forms.ComboBox();
            this.lblDistance = new System.Windows.Forms.Label();
            this.lblMinDistance = new System.Windows.Forms.Label();
            this.lblMaxDistance = new System.Windows.Forms.Label();
            this.btnCopy = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.imgDisplay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMinDistance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMaxDistance)).BeginInit();
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
            // trkPosition
            // 
            this.trkPosition.Location = new System.Drawing.Point(336, 528);
            this.trkPosition.Maximum = 8000;
            this.trkPosition.Minimum = -8000;
            this.trkPosition.Name = "trkPosition";
            this.trkPosition.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trkPosition.Size = new System.Drawing.Size(45, 70);
            this.trkPosition.TabIndex = 4;
            this.trkPosition.TickStyle = System.Windows.Forms.TickStyle.None;
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
            // cmbImageType
            // 
            this.cmbImageType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbImageType.FormattingEnabled = true;
            this.cmbImageType.Items.AddRange(new object[] {
            "Color",
            "Depth",
            "Threshold Depth",
            "Threshold Color"});
            this.cmbImageType.Location = new System.Drawing.Point(222, 495);
            this.cmbImageType.Name = "cmbImageType";
            this.cmbImageType.Size = new System.Drawing.Size(121, 21);
            this.cmbImageType.TabIndex = 7;
            this.cmbImageType.SelectedIndexChanged += new System.EventHandler(this.CmbImageTypeSelectedIndexChanged);
            // 
            // trkMinDistance
            // 
            this.trkMinDistance.Location = new System.Drawing.Point(15, 544);
            this.trkMinDistance.Maximum = 1000;
            this.trkMinDistance.Name = "trkMinDistance";
            this.trkMinDistance.Size = new System.Drawing.Size(104, 45);
            this.trkMinDistance.TabIndex = 8;
            this.trkMinDistance.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkMinDistance.ValueChanged += new System.EventHandler(this.TrkMaxDistanceValueChanged);
            // 
            // trkMaxDistance
            // 
            this.trkMaxDistance.Location = new System.Drawing.Point(179, 544);
            this.trkMaxDistance.Maximum = 1000;
            this.trkMaxDistance.Name = "trkMaxDistance";
            this.trkMaxDistance.Size = new System.Drawing.Size(104, 45);
            this.trkMaxDistance.TabIndex = 9;
            this.trkMaxDistance.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkMaxDistance.Value = 1000;
            this.trkMaxDistance.ValueChanged += new System.EventHandler(this.TrkMaxDistanceValueChanged);
            // 
            // cmbUnits
            // 
            this.cmbUnits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbUnits.FormattingEnabled = true;
            this.cmbUnits.Items.AddRange(new object[] {
            "Centimeters",
            "Inches",
            "Meters",
            "Feet"});
            this.cmbUnits.Location = new System.Drawing.Point(349, 495);
            this.cmbUnits.Name = "cmbUnits";
            this.cmbUnits.Size = new System.Drawing.Size(100, 21);
            this.cmbUnits.TabIndex = 10;
            this.cmbUnits.SelectedIndexChanged += new System.EventHandler(this.CmbUnitsSelectedIndexChanged);
            // 
            // lblDistance
            // 
            this.lblDistance.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDistance.Location = new System.Drawing.Point(113, 547);
            this.lblDistance.Name = "lblDistance";
            this.lblDistance.Size = new System.Drawing.Size(71, 13);
            this.lblDistance.TabIndex = 11;
            this.lblDistance.Text = "DISTANCE";
            this.lblDistance.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblMinDistance
            // 
            this.lblMinDistance.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMinDistance.Location = new System.Drawing.Point(33, 566);
            this.lblMinDistance.Name = "lblMinDistance";
            this.lblMinDistance.Size = new System.Drawing.Size(71, 13);
            this.lblMinDistance.TabIndex = 12;
            this.lblMinDistance.Text = "0 cm";
            this.lblMinDistance.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblMaxDistance
            // 
            this.lblMaxDistance.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMaxDistance.Location = new System.Drawing.Point(196, 566);
            this.lblMaxDistance.Name = "lblMaxDistance";
            this.lblMaxDistance.Size = new System.Drawing.Size(71, 13);
            this.lblMaxDistance.TabIndex = 13;
            this.lblMaxDistance.Text = "1000 cm";
            this.lblMaxDistance.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
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
            this.ClientSize = new System.Drawing.Size(671, 614);
            this.Controls.Add(this.btnCopy);
            this.Controls.Add(this.lblMaxDistance);
            this.Controls.Add(this.lblMinDistance);
            this.Controls.Add(this.lblDistance);
            this.Controls.Add(this.cmbUnits);
            this.Controls.Add(this.trkMaxDistance);
            this.Controls.Add(this.trkMinDistance);
            this.Controls.Add(this.cmbImageType);
            this.Controls.Add(this.lblAccelerometer);
            this.Controls.Add(this.btnExport);
            this.Controls.Add(this.lblDepth);
            this.Controls.Add(this.imgDisplay);
            this.Controls.Add(this.trkPosition);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "NKinect Test";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMainFormClosing);
            this.Load += new System.EventHandler(this.FrmMainLoad);
            ((System.ComponentModel.ISupportInitialize)(this.imgDisplay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMinDistance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMaxDistance)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox imgDisplay;
        private System.Windows.Forms.Label lblDepth;
        private System.Windows.Forms.TrackBar trkPosition;
        private System.Windows.Forms.Button btnExport;
        private System.Windows.Forms.Label lblAccelerometer;
        private System.Windows.Forms.ComboBox cmbImageType;
        private System.Windows.Forms.TrackBar trkMinDistance;
        private System.Windows.Forms.TrackBar trkMaxDistance;
        private System.Windows.Forms.ComboBox cmbUnits;
        private System.Windows.Forms.Label lblDistance;
        private System.Windows.Forms.Label lblMinDistance;
        private System.Windows.Forms.Label lblMaxDistance;
        private System.Windows.Forms.Button btnCopy;
    }
}

