#region
using System;
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
            cmbUnits.SelectedIndex = 0;

            Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.ImageUpdated += KinectImageUpdated;
            Kinect.AccelerometerUpdated += KinectAccelerometerUpdated;
            Kinect.DepthImageUpdated += KinectDepthImageUpdated;
            Kinect.ThresholdDepthImageUpdated += KinectThresholdDepthImageUpdated;
            Kinect.ThresholdColorImageUpdated += KinectThresholdColorImageUpdated;

            Kinect.Start();
        }

        private void KinectImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 0)
                return;

            imgDisplay.Image = e.CameraImage;
            imgDisplay.Invalidate();
        }

        private void KinectDepthImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 1)
                return;

            imgDisplay.Image = e.CameraImage;
            imgDisplay.Invalidate();
        }

        private void KinectThresholdDepthImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 2)
                return;

            imgDisplay.Image = e.CameraImage;
            imgDisplay.Invalidate();
        }

        private void KinectThresholdColorImageUpdated(object sender, CameraImageEventArgs e) {
            if (Mode != 3)
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

            string str = Kinect.DistanceUnit == (Unit) 1
                             ? "in"
                             : (Kinect.DistanceUnit == (Unit) 2 ? "m" : (Kinect.DistanceUnit == (Unit) 3 ? "ft" : "cm"));

            try {
                lblDepth.Text = string.Format("{0} {1}", Math.Max(0, Kinect.GetPreferredUnit(Depths[pnt.X][pnt.Y])).ToString("0.00"), str);
            } catch {
            }
        }

        private void BtnExportClick(object sender, EventArgs e) {
            Kinect.ExportPLY("CurrentView.ply");
        }

        private void CmbImageTypeSelectedIndexChanged(object sender, EventArgs e) {
            Mode = cmbImageType.SelectedIndex;
        }

        private void CmbUnitsSelectedIndexChanged(object sender, EventArgs e) {
            Kinect.DistanceUnit = (Unit) cmbUnits.SelectedIndex;
        }

        private void TrkMaxDistanceValueChanged(object sender, EventArgs e) {
            Kinect.MaxDistanceThreshold = trkMaxDistance.Value;
            Kinect.MinDistanceThreshold = trkMinDistance.Value;

            lblMinDistance.Text = string.Format("{0} cm", trkMinDistance.Value);
            lblMaxDistance.Text = string.Format("{0} cm", trkMaxDistance.Value);
        }

        private void BtnCopyClick(object sender, EventArgs e) {
            Clipboard.SetImage(imgDisplay.Image);
        }
    }
}