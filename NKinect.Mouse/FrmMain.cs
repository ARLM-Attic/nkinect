#region
using System;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using AForge.Imaging;
using AForge.Imaging.Filters;

#endregion

namespace NKinect.Mouse {
    public partial class FrmMain : Form {
        protected double[][] Depths { get; set; }
        protected BaseKinect Kinect { get; set; }

        public FrmMain() {
            InitializeComponent();
        }

        [DllImport("user32.dll")]
        private static extern void mouse_event(uint dwFlags, uint dx, uint dy, uint dwData, int dwExtraInfo);

        private void FrmMainLoad(object sender, EventArgs e) {
            Kinect = KinectFactory.GetKinect();

            Kinect.ThresholdDepthImageUpdated += KinectThresholdDepthImageUpdated;
            Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.MinDistanceThreshold = 100;
            Kinect.MaxDistanceThreshold = 120;

            Kinect.Start();
        }

        private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
            Depths = e.Depths;
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

                var blobs =
                    bc.GetObjectsInformation().Select(blob => blob.Rectangle.GetCenter()).OrderBy(blob => Depths[blob.X][blob.Y]).ToArray();

                switch (blobs.Length) {
                    case 1:
                        var obj = blobs[0];

                        graphics.FillEllipse(new SolidBrush(Color.DarkRed), obj.X, obj.Y, 10, 10);
                        Cursor.Position = MapToScreen(obj);

                        break;

                    case 2:
                        mouse_event((uint) (MouseEventFlags.LEFTDOWN | MouseEventFlags.LEFTUP), (uint) blobs[0].X, (uint) blobs[0].Y, 0, 0);

                        break;

                    default:
                        break;
                }
            }

            imgKinect.Image = e.CameraImage;
            imgKinect.Invalidate();
        }

        private static Point MapToScreen(Point point) {
            return new Point(point.X * Screen.PrimaryScreen.Bounds.Width / 640, point.Y * Screen.PrimaryScreen.Bounds.Height / 480);
        }

        private void TrkPositionScroll(object sender, EventArgs e) {
            Kinect.SetPosition((short) trkPosition.Value);
        }

        private void TrkDistanceScroll(object sender, EventArgs e) {
            Kinect.MinDistanceThreshold = trkMinDistance.Value;
            Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

            lblSize.Text = string.Format("{0} - {1} cm", trkMinDistance.Value, trkMaxDistance.Value);
        }
    }

    public static class Extensions {
        public static Point GetCenter(this Rectangle rect) {
            return new Point(rect.X + rect.Width / 2, rect.Y + rect.Height / 2);
        }
    }
}