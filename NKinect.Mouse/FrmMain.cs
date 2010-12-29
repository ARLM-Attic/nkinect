#region
using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;

#endregion

namespace NKinect.Mouse {
    public partial class FrmMain : Form {
        protected double[][] Depths { get; set; }
        protected bool IsMouseView { get; set; }
        protected BaseKinect Kinect { get; set; }
        protected MouseTracker KinectMouse { get; set; }

        public FrmMain() {
            InitializeComponent();
        }

        [DllImport("user32.dll")]
        private static extern void mouse_event(uint dwFlags, uint dx = 0u, uint dy = 0u, uint dwData = 0u, int dwExtraInfo = 0);

        private void FrmMainLoad(object sender, EventArgs e) {
            Kinect = KinectFactory.GetKinect();
            Kinect.Mirrored = true;

            lblSize.Text = string.Format("{0} - {1} cm", trkMinDistance.Value, trkMaxDistance.Value);

            KinectMouse = new MouseTracker(Kinect);

            KinectMouse.Kinect.ThresholdDepthImageUpdated += KinectThresholdDepthImageUpdated;
            KinectMouse.MouseViewChanged += KinectMouseViewChanged;
            KinectMouse.Kinect.DepthsCalculated += KinectDepthsCalculated;
            KinectMouse.Kinect.MinDistanceThreshold = trkMinDistance.Value;
            KinectMouse.Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

            KinectMouse.Start();
        }

        private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
            Depths = e.Depths;
        }

        private void KinectThresholdDepthImageUpdated(object sender, CameraImageEventArgs e) {
            if (IsMouseView)
                return;

            imgKinect.Image = e.CameraImage;
            imgKinect.Invalidate();
        }

        private void KinectMouseViewChanged(object sender, CameraImageEventArgs e) {
            if (!IsMouseView)
                return;

            imgKinect.Image = e.CameraImage;
            imgKinect.Invalidate();
        }

        private void TrkPositionScroll(object sender, EventArgs e) {
            KinectMouse.Kinect.SetPosition((short) trkPosition.Value);
        }

        private void TrkDistanceScroll(object sender, EventArgs e) {
            KinectMouse.Kinect.MinDistanceThreshold = trkMinDistance.Value;
            KinectMouse.Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

            lblSize.Text = string.Format("{0} - {1} cm", trkMinDistance.Value, trkMaxDistance.Value);
        }

        private void BtnAutoClick(object sender, EventArgs e) {
            trkMaxDistance.Value = trkMaxDistance.Maximum;
            trkMinDistance.Value = trkMinDistance.Minimum;

            KinectMouse.Kinect.MinDistanceThreshold = trkMinDistance.Value;
            KinectMouse.Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

            double closest = 9000;

            foreach (double t1 in Depths.SelectMany(t => t.Where(t1 => t1 < closest && t1 > 20)))
                closest = t1;

            SyncToDistance(closest);
        }

        private void SyncToDistance(double closest) {
            if (closest <= 20)
                return;

            trkMaxDistance.Value = (int) (closest + 5);
            trkMinDistance.Value = (int) (closest - 5);

            lblSize.Text = string.Format("{0} - {1} cm", trkMinDistance.Value, trkMaxDistance.Value);
        }

        private void ImgKinectMouseDown(object sender, MouseEventArgs e) {
            SyncToDistance(Depths[e.X][e.Y]);
        }

        private void ChkMouseCheckedChanged(object sender, EventArgs e) {
            KinectMouse.Enabled = chkMouse.Checked;
        }

        private void ChkMouseViewCheckedChanged(object sender, EventArgs e) {
            IsMouseView = chkMouseView.Checked;
        }
    }
}