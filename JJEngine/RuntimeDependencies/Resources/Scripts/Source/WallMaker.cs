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
        int numOfWall = 0;
        float timer = 0.0f;
        Vector3 velocity = Vector3.Zero;

        protected override void OnCreate()
        {
            velocity.X = -3.0f;
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
                Entity entity = Scene.CreateEntity("Test" + numOfWall++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = -2.0f + a;
                wallPos.Z = 0.0f;
                entity.Position = wallPos;
                
                //Give class "Wall"
                //auto & script = entity.AddComponent<ScriptComponent>();
                //script.Name = "Game.Player";


                entity = Scene.CreateEntity("Test" + numOfWall++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = 2.0f + a;
                wallPos.Z = 0.0f;
                entity.Position = wallPos;
            }

            for(int i = 0; i < numOfWall; i++)
            {
                Entity wall = Scene.GetEntity("Test" + i.ToString());
                if (wall != null)
                {
                    TargetWallTransform = wall.GetComponent<TransformComponent>();
                    TargetWallTransform.Position += velocity * Time.Delta;

                }
            }

        }
    }
}
