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
            this.trkDistance = new System.Windows.Forms.TrackBar();
            this.lblSize = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.imgKinect)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkPosition)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trkDistance)).BeginInit();
            this.SuspendLayout();
            // 
            // imgKinect
            // 
            this.imgKinect.Location = new System.Drawing.Point(11, 12);
            this.imgKinect.Name = "imgKinect";
            this.imgKinect.Size = new System.Drawing.Size(640, 480);
            this.imgKinect.TabIndex = 0;
            this.imgKinect.TabStop = false;
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
            // trkDistance
            // 
            this.trkDistance.Location = new System.Drawing.Point(3, 498);
            this.trkDistance.Maximum = 400;
            this.trkDistance.Minimum = 100;
            this.trkDistance.Name = "trkDistance";
            this.trkDistance.Size = new System.Drawing.Size(575, 45);
            this.trkDistance.TabIndex = 6;
            this.trkDistance.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trkDistance.Value = 120;
            this.trkDistance.Scroll += new System.EventHandler(this.TrkDistanceScroll);
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
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(687, 529);
            this.Controls.Add(this.lblSize);
            this.Controls.Add(this.trkDistance);
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
            ((System.ComponentModel.ISupportInitialize)(this.trkDistance)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox imgKinect;
        private System.Windows.Forms.TrackBar trkPosition;
        private System.Windows.Forms.TrackBar trkDistance;
        private System.Windows.Forms.Label lblSize;
    }
}

