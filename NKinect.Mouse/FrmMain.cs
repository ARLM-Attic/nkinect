#region
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using AForge;
using AForge.Imaging;
using AForge.Imaging.Filters;
using AForge.Math.Geometry;

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
            Kinect.Mirrored = true;

            lblSize.Text = string.Format("{0} - {1} cm", trkMinDistance.Value, trkMaxDistance.Value);

            Kinect.ThresholdDepthImageUpdated += KinectThresholdDepthImageUpdated;
            Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.MinDistanceThreshold = trkMinDistance.Value;
            Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

            Kinect.Start();
        }

        private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
            Depths = e.Depths;
        }

        private void KinectThresholdDepthImageUpdated(object sender, CameraImageEventArgs e) {
            var gray = new Grayscale(0.2125, 0.7154, 0.0721);
            var grayImage = gray.Apply(e.CameraImage);

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

                var hullFinder = new GrahamConvexHull();
                filter.ApplyInPlace(grayImage);

                bc.ProcessImage(grayImage);

                var blobs =
                    bc.GetObjectsInformation().OrderBy(blob => Depths[blob.CenterOfGravity.X][blob.CenterOfGravity.Y]).ToArray();

                foreach (var blob in blobs) {
                    List<IntPoint> leftPoints, rightPoints;
                    var edgePoints = new List<IntPoint>();

                    bc.GetBlobsLeftAndRightEdges(blob, out leftPoints, out rightPoints);

                    edgePoints.AddRange(leftPoints);
                    edgePoints.AddRange(rightPoints);

                    var hull = hullFinder.FindHull(edgePoints);

                    graphics.FillEllipse(new SolidBrush(Color.DarkRed), blob.CenterOfGravity.X, blob.CenterOfGravity.Y, 10, 10);
                    graphics.DrawPolygon(new Pen(Color.Blue, 2f), hull.Select(h => new Point(h.X, h.Y)).ToArray());

                    foreach (var pnt in hull)
                        graphics.FillEllipse(new SolidBrush(Color.ForestGreen), pnt.X, pnt.Y, 8, 8);

                    if (chkMouse.Checked && blobs.Length == 1)
                        Cursor.Position = MapToScreen(blob.CenterOfGravity);
                }
            }

            imgKinect.Image = e.CameraImage;
            imgKinect.Invalidate();
        }

        private static Point MapToScreen(IntPoint point) {
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

        private void BtnAutoClick(object sender, EventArgs e) {
            trkMaxDistance.Value = trkMaxDistance.Maximum;
            trkMinDistance.Value = trkMinDistance.Minimum;

            Kinect.MinDistanceThreshold = trkMinDistance.Value;
            Kinect.MaxDistanceThreshold = trkMaxDistance.Value;

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
    }
}