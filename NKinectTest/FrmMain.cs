#region
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using NKinect;

#endregion

namespace NKinectTest {
    public partial class FrmMain : Form {
        protected BaseKinect Kinect;
        public double[][] Depths { get; set; }

        public FrmMain() {
            InitializeComponent();

            Kinect = NKinect.Kinect.GetKinect();
        }

        private void FrmMainLoad(object sender, EventArgs e) {
            Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.ImageUpdated += KinectImageUpdated;
            Kinect.AccelerometerUpdated += KinectAccelerometerUpdated;

            Kinect.Start();
        }

        private void KinectImageUpdated(object sender, CameraImageEventArgs e) {
            imgColor.Image = e.CameraImage;
            imgColor.Invalidate();
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

            Console.WriteLine("{0} important pixels", list.Count);
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
    }
}