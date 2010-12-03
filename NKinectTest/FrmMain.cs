#region
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using NKinect;

#endregion

namespace NKinectTest {
    public partial class FrmMain : Form {
        protected BaseKinect Kinect;
        public double[][] Depths { get; set; }
        protected int Mode { get; set; }

        public FrmMain() {
            InitializeComponent();

            Kinect = KinectFactory.GetKinect();
        }

        private void FrmMainLoad(object sender, EventArgs e) {
            cmbImageType.SelectedIndex = 0;
            
            Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.ImageUpdated += KinectImageUpdated;
            Kinect.AccelerometerUpdated += KinectAccelerometerUpdated;
            Kinect.DepthImageUpdated += KinectDepthImageUpdated;

            Kinect.Start();
        }

        private void KinectDepthImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 1)
                return;

            imgDisplay.Image = e.CameraImage;
            imgDisplay.Invalidate();
        }

        private void KinectImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 0)
                return;

            imgDisplay.Image = e.CameraImage;
            imgDisplay.Invalidate();
        }

        private void KinectAccelerometerUpdated(object sender, AccelerometerEventArgs e) {
            try {
                Invoke(new EventHandler(delegate { lblAccelerometer.Text = string.Format("{0}, {1}, {2}", e.X, e.Y, e.Z); }));
                Invalidate();
                Application.DoEvents();
            } catch {
            }
        }

        private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
            Depths = e.Depths;

            var list = new List<Point>();

            for (int y = 0; y < Depths[0].Length; y++) {
                for (int x = 0; x < Depths.Length; x++) {
                    if (Depths[x][y] > 0)
                        list.Add(new Point(x, y));
                }
            }
        }

        private void FrmMainFormClosing(object sender, FormClosingEventArgs e) {
            Kinect.Stop();
        }

        private void TrkPositionValueChanged(object sender, EventArgs e) {
            Kinect.SetPosition((short) trkPosition.Value);
        }

        private void ImgColorMouseMove(object sender, MouseEventArgs e) {
            var pnt = e.Location;

            if (Depths == null)
                return;

            lblDepth.Text = string.Format("{0} cm", Depths[pnt.X][pnt.Y].ToString("0.00"));
        }

        private void BtnExportClick(object sender, EventArgs e) {
            Kinect.ExportPLY("CurrentView.ply");
        }

        private void CmbImageTypeSelectedIndexChanged(object sender, EventArgs e) {
            Mode = cmbImageType.SelectedIndex;
        }
    }
}