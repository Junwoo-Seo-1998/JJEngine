using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JJEngine;

namespace Game
{
    public class CameraControl  : Entity
    {
        private TransformComponent m_transform;
        protected override void OnCreate()
        {
            m_transform = GetComponent<TransformComponent>();
        }

        protected override void OnUpdate()
        {
            Vector3 velocity = Vector3.Zero;
            if (Input.IsPressed(KeyCode.Up))
            {
                velocity.Y += 1.0f;

            }
            else if (Input.IsPressed(KeyCode.Down))
            {
                velocity.Y -= 1.0f;

            }


            if (Input.IsPressed(KeyCode.Right))
            {
                velocity.X += 1.0f;

            }
            else if (Input.IsPressed(KeyCode.Left))
            {
                velocity.X -= 1.0f;
            }

            float speed = 3.0f;
            velocity *= speed;
            m_transform.Position += velocity * Time.Delta;
        }
    }
}
