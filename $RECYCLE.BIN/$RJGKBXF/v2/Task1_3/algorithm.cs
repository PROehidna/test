using System;

namespace arch_algo
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("исходные биты 11101110 \n" +
                "в десятичной 238" +
                "в 14-ричной 130"); //сверяться по numsys.ru
            double b = 14; // разрядность в варианте
            Console.ReadLine();
            double[] bit = { 1, 1, 0, 0, 1, 1, 0, 0 }; //sw1 - 1 - sw2 - 0 - 1 - sw3 - sw4 - 0
            double dec = 0;
            int j = 7;
            for (int i = 0; i < 8; i++)
            {
                dec += (bit[i] * Math.Pow(2, j));
                j--;
            }
            Console.WriteLine("число в десятичке - " + dec + " " + j);
            double div;
            double[] mas = new double[8];
            for (int i = 0; i < 9; i++)
            {
                div = dec % b;
                mas[i] = div;
                dec -= div;
                dec = dec / b;
                if (dec < b)
                {
                    i++;
                    mas[i] = dec;
                    if (dec < 14)
                        break;
                }
            }
            double t = 0;
            for (int i = 7; i >= 0; i--)
            {
                if (t == 0)
                {
                    t = mas[i];
                    if (t !=0)
                    {
                        Console.WriteLine(mas[i]);
                    }
                }
                else
                    Console.WriteLine(mas[i]);
            }
            Console.ReadLine();
        }
    }   
}
    

