#region
using System.Windows.Forms;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using ButtonState = Microsoft.Xna.Framework.Input.ButtonState;
using Keys = Microsoft.Xna.Framework.Input.Keys;

#endregion

namespace NKinect.XNA {
    public class KinectCloud : Game {
        protected Vector3 CameraPosition = new Vector3(0.0f, 50.0f, 30000.0f);
        protected Vector3 Position;
        protected float Zoom;
        protected float AspectRatio { get; set; }
        protected Model Cube { get; set; }
        protected GraphicsDeviceManager Graphics { get; set; }
        protected BaseKinect Kinect { get; set; }
        protected float ModelRotation { get; set; }
        protected Vector3[] Positions { get; set; }
        protected SpriteBatch SpriteBatch { get; set; }
        
        public KinectCloud() {
            Graphics = new GraphicsDeviceManager(this) {
                IsFullScreen = true,
                PreferredBackBufferWidth = Screen.PrimaryScreen.Bounds.Width,
                PreferredBackBufferHeight = Screen.PrimaryScreen.Bounds.Height
            };

            Content.RootDirectory = "Content";
            Kinect = KinectFactory.GetKinect();
        }

        protected override void Initialize() {
            //Positions = new Vector3[1000];

            //for (int i = 0; i < Positions.Length; i++) {
            //    var p = Positions[i];

            //    p.X = i;
            //    p.Y = i;
            //    p.Z = 0;
            //}

            Position = Vector3.Zero;

            base.Initialize();
        }

        protected override void LoadContent() {
            SpriteBatch = new SpriteBatch(GraphicsDevice);

            AspectRatio = Graphics.GraphicsDevice.Viewport.AspectRatio;
            Cube = Content.Load<Model>("cube");
        }

        protected override void UnloadContent() {
        }

        protected override void Update(GameTime gameTime) {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed ||
                Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Escape))
                Exit();

            if (Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Right))
                Position.X += 10;

            if (Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Left))
                Position.X -= 10;

            if (Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Up))
                Position.Y += 10;

            if (Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Down))
                Position.Y -= 10;

            CameraPosition.Z = 30000.0f - (Mouse.GetState().ScrollWheelValue * 10);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime) {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            var transforms = new Matrix[Cube.Bones.Count];
            Cube.CopyAbsoluteBoneTransformsTo(transforms);

            //foreach (var p in Positions) {
            foreach (var mesh in Cube.Meshes) {
                foreach (BasicEffect effect in mesh.Effects) {
                    effect.EnableDefaultLighting();
                    effect.World = transforms[mesh.ParentBone.Index] * Matrix.CreateRotationY(ModelRotation) *
                                   Matrix.CreateTranslation(Position);
                    effect.View = Matrix.CreateLookAt(CameraPosition, Vector3.Zero, Vector3.Up);
                    effect.Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45.0f), AspectRatio, 1.0f, 100000.0f);
                }

                mesh.Draw();
            }
            //}

            base.Draw(gameTime);
        }
    }
}