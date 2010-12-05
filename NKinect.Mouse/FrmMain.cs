#region
using System;
using System.Drawing;
using System.Windows.Forms;
using AForge.Imaging;
using AForge.Imaging.Filters;

#endregion

namespace NKinect.Mouse {
    public partial class FrmMain : Form {
        protected BaseKinect Kinect { get; set; }

        public FrmMain() {
            InitializeComponent();
        }

        private void FrmMainLoad(object sender, EventArgs e) {
            Kinect = KinectFactory.GetKinect();

            Kinect.ThresholdDepthImageUpdated += KinectThresholdDepthImageUpdated;
            Kinect.MinDistanceThreshold = 100;
            Kinect.MaxDistanceThreshold = 120;

            Kinect.Start();
        }

        private void KinectThresholdDepthImageUpdated(object sender, CameraImageEventArgs e) {
            using (var graphics = Graphics.FromImage(e.CameraImage)) {
                var bc = new BlobCounter {
                    FilterBlobs = true,
                    MinWidth = 5,
                    MinHeight = 5,
                    ObjectsOrder = ObjectsOrder.Size
                };
                var filter = new BlobsFiltering {
                    CoupledSizeFiltering = true,
                    MinWidth = 70,
                    MinHeight = 70
                };

                filter.ApplyInPlace(e.CameraImage);
                bc.ProcessImage(e.CameraImage);

                var blobs = bc.GetObjectsInformation();

                if (blobs.Length == 1) {
                    var blob = blobs[0];
                    var center = blob.Rectangle.GetCenter();

                    graphics.FillEllipse(new SolidBrush(Color.DarkRed), center.X, center.Y, 10, 10);

                    Cursor.Position = center;
                }
            }

            imgKinect.Image = e.CameraImage;
            imgKinect.Invalidate();
        }

        private void TrkPositionScroll(object sender, EventArgs e) {
            Kinect.SetPosition((short) trkPosition.Value);
        }

        private void TrkDistanceScroll(object sender, EventArgs e) {
            Kinect.MinDistanceThreshold = trkDistance.Value;
            Kinect.MaxDistanceThreshold = trkDistance.Value + 20;

            lblSize.Text = string.Format("{0} - {1} cm", trkDistance.Value, trkDistance.Value + 20);
        }
    }

    public static class Extensions {
        public static Point GetCenter(this Rectangle rect) {
            return new Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
        }
    }
}