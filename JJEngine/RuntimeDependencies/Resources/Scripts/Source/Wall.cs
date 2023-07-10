using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JJEngine;

namespace Game
{
    internal class Wall : Entity
    {
        private TransformComponent m_transform;
        private RigidBody2DComponent m_rigidBody;

        Vector3 velocity = Vector3.Zero;

        protected override void OnCreate()
        {
            Console.WriteLine($"Wall create");
            m_transform = GetComponent<TransformComponent>();
            m_rigidBody = GetComponent<RigidBody2DComponent>();
        }

        protected override void OnUpdate()
        {
            velocity.X = -5.0f;


            m_transform.Position += velocity * Time.Delta;
        }
    }
}
