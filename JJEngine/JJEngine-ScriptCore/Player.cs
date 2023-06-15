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
        void OnCreate()
        {
            Console.WriteLine("Player create");
        }
        void OnUpdate()
        {
            Console.WriteLine("Player update ");
        }

    }
}
