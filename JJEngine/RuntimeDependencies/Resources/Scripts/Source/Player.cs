using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JJEngine;
namespace Game
{
    class Player : Entity
    {
        private TransformComponent m_transform;
        private RigidBody2DComponent m_rigidBody;

        private int i = 0;

        public float Speed =60.0f;
        public float ttttt = 3.0f;
        Vector3 velocity = Vector3.Zero;

        protected override void OnCreate()
        {
            Console.WriteLine($"Player create");
            m_transform = GetComponent<TransformComponent>();
            m_rigidBody = GetComponent<RigidBody2DComponent>();

            Vector3 wallPos;
            Entity entity = Scene.CreateEntity("Test" + i++.ToString());
            entity.AddComponent<SpriteRendererComponent>();
            wallPos.X = 1.0f;
            wallPos.Y = -2.0f;
            wallPos.Z = 0.0f;
            entity.Position = wallPos;

            //Give class "Wall"
            //auto & script = entity.AddComponent<ScriptComponent>();
            //script.Name = "Game.Player";


            entity = Scene.CreateEntity("Test" + i++.ToString());
            entity.AddComponent<SpriteRendererComponent>();
            wallPos.X = 1.0f;
            wallPos.Y = 2.0f;
            wallPos.Z = 0.0f;
            entity.Position = wallPos;

            entity = Scene.CreateEntity("Test" + i++.ToString());
            entity.AddComponent<SpriteRendererComponent>();
            wallPos.X = 3.0f;
            wallPos.Y = -2.0f;
            wallPos.Z = 0.0f;
            entity.Position = wallPos;

            //Give class "Wall"
            //auto & script = entity.AddComponent<ScriptComponent>();
            //script.Name = "Game.Player";


            entity = Scene.CreateEntity("Test" + i++.ToString());
            entity.AddComponent<SpriteRendererComponent>();
            wallPos.X = 3.0f;
            wallPos.Y = 2.0f;
            wallPos.Z = 0.0f;
            entity.Position = wallPos;
        }

        protected override void OnUpdate()
        {
            float gravity = 0.3f;



            if (Input.IsPressed(KeyCode.W))
            {
                velocity.Y += 1.0f;

            }
            else if (Input.IsPressed(KeyCode.S))
            {
                velocity.Y -= 1.0f;

            }


            if (Input.IsPressed(KeyCode.D))
            {
                velocity.X += 1.0f;

            }
            else if (Input.IsPressed(KeyCode.A))
            {
                velocity.X -= 1.0f;
            }

            if(Input.IsTriggered(KeyCode.Space))
            {
                velocity.Y = 8.0f;
            }

            if (Input.IsPressed(KeyCode.Space))
            {
                Vector3 wallPos;
                Entity entity = Scene.CreateEntity("Test" + i++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = -2.0f;
                wallPos.Z = 0.0f;
                entity.Position = wallPos;

                //Give class "Wall"
                //auto & script = entity.AddComponent<ScriptComponent>();
                //script.Name = "Game.Player";


                entity = Scene.CreateEntity("Test" + i++.ToString());
                entity.AddComponent<SpriteRendererComponent>();
                wallPos.X = 2.0f;
                wallPos.Y = 2.0f;
                wallPos.Z = 0.0f;
                entity.Position = wallPos;
            }

            velocity.Y -= gravity;

            /*if (m_rigidBody != null)
            {
                m_rigidBody.ApplyLinearImpulse(velocity.XY, true);
            }*/
            m_transform.Position += velocity * Time.Delta;
        }

    }
}
