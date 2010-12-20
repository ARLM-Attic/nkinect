namespace NKinect.Mouse {
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
            this.imgKinect = new System.Windows.Forms.PictureBox();
            this.trkPosition = new System.Windows.Forms.TrackBar();
            this.trkMinDistance = new System.Windows.Forms.TrackBar();
            this.lblSize = new System.Windows.Forms.Label();
            this.trkMaxDistance = new System.Windows.Forms.TrackBar();
            this.btnAuto = new System.Windows.Forms.Button();
            this.chkMouse = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.imgKinect)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMinDistance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMaxDistance)).BeginInit();
            this.SuspendLayout();
            // 
            // imgKinect
            // 
            this.imgKinect.Location = new System.Drawing.Point(11, 12);
            this.imgKinect.Name = "imgKinect";
            this.imgKinect.Size = new System.Drawing.Size(640, 480);
            this.imgKinect.TabIndex = 0;
            this.imgKinect.TabStop = false;
            this.imgKinect.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ImgKinectMouseDown);
            // 
            // trkPosition
            // 
            this.trkPosition.Location = new System.Drawing.Point(656, 4);
            this.trkPosition.Maximum = 8000;
            this.trkPosition.Minimum = -8000;
            this.trkPosition.Name = "trkPosition";
            this.trkPosition.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trkPosition.Size = new System.Drawing.Size(45, 497);
            this.trkPosition.TabIndex = 5;
            this.trkPosition.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkPosition.Scroll += new System.EventHandler(this.TrkPositionScroll);
            // 
            // trkMinDistance
            // 
            this.trkMinDistance.Location = new System.Drawing.Point(3, 498);
            this.trkMinDistance.Maximum = 900;
            this.trkMinDistance.Minimum = 20;
            this.trkMinDistance.Name = "trkMinDistance";
            this.trkMinDistance.Size = new System.Drawing.Size(289, 45);
            this.trkMinDistance.TabIndex = 6;
            this.trkMinDistance.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkMinDistance.Value = 20;
            this.trkMinDistance.Scroll += new System.EventHandler(this.TrkDistanceScroll);
            // 
            // lblSize
            // 
            this.lblSize.AutoSize = true;
            this.lblSize.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSize.Location = new System.Drawing.Point(579, 501);
            this.lblSize.Name = "lblSize";
            this.lblSize.Size = new System.Drawing.Size(71, 13);
            this.lblSize.TabIndex = 7;
            this.lblSize.Text = "100 - 120 cm";
            // 
            // trkMaxDistance
            // 
            this.trkMaxDistance.Location = new System.Drawing.Point(292, 498);
            this.trkMaxDistance.Maximum = 900;
            this.trkMaxDistance.Minimum = 20;
            this.trkMaxDistance.Name = "trkMaxDistance";
            this.trkMaxDistance.Size = new System.Drawing.Size(289, 45);
            this.trkMaxDistance.TabIndex = 8;
            this.trkMaxDistance.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkMaxDistance.Value = 900;
            this.trkMaxDistance.ValueChanged += new System.EventHandler(this.TrkDistanceScroll);
            // 
            // btnAuto
            // 
            this.btnAuto.Location = new System.Drawing.Point(95, 515);
            this.btnAuto.Name = "btnAuto";
            this.btnAuto.Size = new System.Drawing.Size(68, 23);
            this.btnAuto.TabIndex = 9;
            this.btnAuto.Text = "Auto";
            this.btnAuto.UseVisualStyleBackColor = true;
            this.btnAuto.Click += new System.EventHandler(this.BtnAutoClick);
            // 
            // chkMouse
            // 
            this.chkMouse.AutoSize = true;
            this.chkMouse.Location = new System.Drawing.Point(11, 519);
            this.chkMouse.Name = "chkMouse";
            this.chkMouse.Size = new System.Drawing.Size(61, 17);
            this.chkMouse.TabIndex = 10;
            this.chkMouse.Text = "Mouse";
            this.chkMouse.UseVisualStyleBackColor = true;
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(687, 545);
            this.Controls.Add(this.chkMouse);
            this.Controls.Add(this.btnAuto);
            this.Controls.Add(this.lblSize);
            this.Controls.Add(this.trkMaxDistance);
            this.Controls.Add(this.trkMinDistance);
            this.Controls.Add(this.trkPosition);
            this.Controls.Add(this.imgKinect);
            this.Font = new System.Drawing.Font("Segoe UI", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "NKinect: Mouse Emulation";
            this.Load += new System.EventHandler(this.FrmMainLoad);
            ((System.ComponentModel.ISupportInitialize)(this.imgKinect)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMinDistance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkMaxDistance)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox imgKinect;
        private System.Windows.Forms.TrackBar trkPosition;
        private System.Windows.Forms.TrackBar trkMinDistance;
        private System.Windows.Forms.Label lblSize;
        private System.Windows.Forms.TrackBar trkMaxDistance;
        private System.Windows.Forms.Button btnAuto;
        private System.Windows.Forms.CheckBox chkMouse;
    }
}

