#region
using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

#endregion

namespace NKinect.XNA {
    public class KinectCloud : Game {
        protected double[][] Depths { get; set; }
        protected GraphicsDeviceManager Graphics { get; set; }
        protected BaseKinect Kinect { get; set; }
        protected float ModelRotation { get; set; }
        protected Texture2D RectTexture { get; set; }
        protected SpriteBatch SpriteBatch { get; set; }

        public KinectCloud() {
            Graphics = new GraphicsDeviceManager(this) {
                IsFullScreen = true,
                PreferredBackBufferWidth = 640,
                PreferredBackBufferHeight = 480
            };

            Content.RootDirectory = "Content";
            Kinect = KinectFactory.GetKinect();

            Kinect.DepthsCalculated += KinectDepthsCalculated;

            Kinect.Start();
        }

        private void KinectDepthsCalculated(object sender, DepthEventArgs e) {
            Depths = e.Depths;
        }

        protected override void Initialize() {
            Window.Title = "NKinect + XNA Test";

            base.Initialize();
        }

        protected override void LoadContent() {
            SpriteBatch = new SpriteBatch(GraphicsDevice);

            RectTexture = Content.Load<Texture2D>("rect");
        }

        protected override void UnloadContent() {
        }

        protected override void Update(GameTime gameTime) {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed ||
                Keyboard.GetState(PlayerIndex.One).IsKeyDown(Keys.Escape))
                Exit();

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime) {
            GraphicsDevice.Clear(Color.Black);

            SpriteBatch.Begin();

            const int minDistance = -10;
            const double scaleFactor = 0.0021;

            if (Depths != null) {
                for (int j = 0; j < 480; j++) {
                    for (int i = 0; i < 640; i++) {
                        double z = Depths[i][j];

                        if (z > 1500)
                            continue;

                        double y = (640 / 2 - j) * (z + minDistance) * scaleFactor;
                        double x = (i - 480 / 2) * (z + minDistance) * scaleFactor;
                        var newDepth = (float) (Math.Abs(z) / 1000);
                        int color = (int) (newDepth * 1000) / 4;

                        SpriteBatch.Draw(RectTexture, new Vector2((float) x + 240, (float) (320 - y)), null, new Color(color, color, color), 0, Vector2.Zero, Vector2.One,
                                         SpriteEffects.None, newDepth);
                    }
                }
            }

            SpriteBatch.End();

            base.Draw(gameTime);
        }
    }
}