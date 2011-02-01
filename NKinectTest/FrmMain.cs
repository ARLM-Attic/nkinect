#region
using System;
using System.Drawing;
using System.Windows.Forms;
using NKinect;

#endregion

namespace NKinectTest {
    public partial class FrmMain : Form {
        protected Sensor Kinect;
        public double[][] Depths { get; set; }
        protected int Mode { get; set; }

        public FrmMain() {
            InitializeComponent();

            Kinect = new Sensor(); //KinectFactory.GetKinect();
        }

        private void FrmMainLoad(object sender, EventArgs e) {
            cmbImageType.SelectedIndex = 1;
            
            //Kinect.DepthsCalculated += KinectDepthsCalculated;
            Kinect.ImageUpdated += KinectImageUpdated;
            Kinect.DepthImageUpdated += KinectDepthImageUpdated;

            Kinect.AddControl(new Slider2DControl(1920, 1200, (x, y) => { Cursor.Position = new Point(x, y); }));
            Kinect.AddControl(new PushControl(() => { Console.WriteLine("Push detected"); }));
            Kinect.AddControl(new SteadyControl(5, () => {
                                                       Console.WriteLine("Steady hand detected");
                                                       Win32Helper.SendLeftMouseClick();
                                                   }));
            Kinect.AddControl(new SwipeControl(() => Console.WriteLine("Swipe detected: UP"),
                                               () => Console.WriteLine("Swipe detected: DOWN"),
                                               () => Console.WriteLine("Swipe detected: RIGHT"),
                                               () => Console.WriteLine("Swipe detected: LEFT")));
            Kinect.AddControl(new WaveControl(() => Console.WriteLine("Wave detected")));
            Kinect.AddControl(new CircleControl(() => Console.WriteLine("Circle detected")));

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

        //private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
        //    Depths = e.Depths;
        //}

        private void FrmMainFormClosing(object sender, FormClosingEventArgs e) {
            Kinect.Stop();
        }

        private void ImgColorMouseMove(object sender, MouseEventArgs e) {
            var pnt = e.Location;

            if (Depths == null)
                return;

            //string str = Kinect.DistanceUnit == Unit.Inches
            //                 ? "in"
            //                 : (Kinect.DistanceUnit == Unit.Meters ? "m" : (Kinect.DistanceUnit == 3 ? "ft" : "cm"));

            //try {
            //    lblDepth.Text = string.Format("{0} {1}", Math.Max(0, Kinect.GetPreferredUnit(Depths[pnt.X][pnt.Y])).ToString("0.00"), str);
            //} catch {
            //}
        }

        private void BtnExportClick(object sender, EventArgs e) {
            //Kinect.ExportPLY("CurrentView.ply");
        }

        private void CmbImageTypeSelectedIndexChanged(object sender, EventArgs e) {
            Mode = cmbImageType.SelectedIndex;
        }

        private void BtnCopyClick(object sender, EventArgs e) {
            Clipboard.SetImage(imgDisplay.Image);
        }
    }
}