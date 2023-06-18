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
        protected override void OnCreate()
        {
            Console.WriteLine($"Player create");
            m_transform = GetComponent<TransformComponent>();
        }

        protected override void OnUpdate()
        {
            Vector3 velocity=Vector3.Zero;
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

            float speed = 3.0f;
            velocity *= speed;
            m_transform.Position += velocity * Time.Delta;
        }

    }
}
