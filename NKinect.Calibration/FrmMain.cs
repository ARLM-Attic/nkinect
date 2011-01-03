#region
using System;
using System.Windows.Forms;

#endregion

namespace NKinect.Calibration {
    public partial class FrmMain : Form {
        protected BaseKinect Kinect { get; set; }
        protected KinectCalibrator Calibrator { get; set; }

        public FrmMain() {
            InitializeComponent();
        }

        private void FrmMainLoad(object sender, EventArgs e) {
            Kinect = KinectFactory.GetKinect();
            Calibrator = new KinectCalibrator(Kinect);

            Calibrator.Kinect.ImageUpdated += KinectImageUpdated;
            Calibrator.CalibratedImageUpdated += CalibratedImageUpdated;

            Calibrator.Start();
        }

        private void KinectImageUpdated(object sender, CameraImageEventArgs e) {
            picColor.Image = e.CameraImage;
            picColor.Invalidate();
        }

        private void CalibratedImageUpdated(object sender, CameraImageEventArgs e) {
            picCalibrated.Image = e.CameraImage;
            picCalibrated.Invalidate();
        }
    }
}