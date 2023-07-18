using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JJEngine;

namespace Game
{
    class WallMaker : Entity
    {
        private TransformComponent TargetWallTransform;
        private TransformComponent m_playerTransform;

        bool gameOver = false;
        int numOfWall = 0;
        float timer = 0.0f;
        Vector3 velocity = Vector3.Zero;

        protected override void OnCreate()
        {
            velocity.X = -3.0f;

            Entity player = Scene.GetEntity("Player");
            if (player != null)
            {
                m_playerTransform = player.GetComponent<TransformComponent>();
            }
        }

        protected override void OnUpdate()
        {
            timer += Time.Delta;

            if(timer >= 1.5f)
            {
                System.Random ranNum = new System.Random();
                int a = ranNum.Next(-3, 3);
                timer = 0.0f;
                Vector3 wallPos;
                Vector3 wallSize;
                wallSize.X = 0.5f;
                wallSize.Y = 10.0f;
                wallSize.Z = 1.0f;
                Entity entity = Scene.CreateEntity("Test" + numOfWall++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = -7.0f + a;
                wallPos.Z = 0.0f;               
                entity.Position = wallPos;     
                entity.Scale = wallSize;

                entity = Scene.CreateEntity("Test" + numOfWall++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = 7.0f + a;
                wallPos.Z = 0.0f;
                entity.Position = wallPos;
                entity.Scale = wallSize;
            }

            for(int i = 0; i < numOfWall; i++)
            {
                Entity wall = Scene.GetEntity("Test" + i.ToString());
                if (wall != null)
                {
                    TargetWallTransform = wall.GetComponent<TransformComponent>();
                    if(TargetWallTransform != null)
                    {
                        bool playerRightCollidedCheck = m_playerTransform.Position.X + (m_playerTransform.Scale.X / 2.0f) > TargetWallTransform.Position.X - (TargetWallTransform.Scale.X / 2.0f);
                        bool playerLeftCollidedCheck = m_playerTransform.Position.X - (m_playerTransform.Scale.X / 2.0f) < TargetWallTransform.Position.X + (TargetWallTransform.Scale.X / 2.0f);
                        bool playerTopCollidedCheck = m_playerTransform.Position.Y + (m_playerTransform.Scale.Y / 2.0f) > TargetWallTransform.Position.Y - (TargetWallTransform.Scale.Y / 2.0f);
                        bool playerBottomCollidedCheck = m_playerTransform.Position.Y - (m_playerTransform.Scale.Y / 2.0f) < TargetWallTransform.Position.Y + (TargetWallTransform.Scale.Y / 2.0f);

                        if (playerRightCollidedCheck && playerLeftCollidedCheck && playerTopCollidedCheck && playerBottomCollidedCheck)
                        {
                            gameOver = true;
                        }

                        if (!gameOver)
                        {
                            TargetWallTransform.Position += velocity * Time.Delta;
                        }
                    }
                   
                }

            }

        }
    }
}
