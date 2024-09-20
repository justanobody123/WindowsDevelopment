using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Clock
{
    public partial class ChooseFont : Form
    {
        public ChooseFont()
        {
            InitializeComponent();
            Console.WriteLine(Directory.GetCurrentDirectory());
            string[] ttfFiles = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.ttf");
            string[] otfFiles = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.otf");
            string[] files = new string[ttfFiles.Length + otfFiles.Length];
            ttfFiles.CopyTo(files, 0);
            otfFiles.CopyTo(files, ttfFiles.Length);
            for (int i = 0; i < ttfFiles.Length; i++)
            {
                comboBoxFonts.Items.Add(files[i].Split('\\').Last());

            }
            //string[] extensions = new string[] { ".ttf", ".otf" };
            //string[] files = Directory.GetFiles(Directory.GetCurrentDirectory()).Where(item=> extensions.Contains(item.Split().Last('\\')));
        }

        private void comboBoxFonts_SelectedIndexChanged(object sender, EventArgs e)
        {
            PrivateFontCollection pfc = new PrivateFontCollection();
            pfc.AddFontFile(comboBoxFonts.SelectedItem.ToString());
            Font font = new Font(pfc.Families[0], Convert.ToInt32(numericUpDownFontSize.Value));
            pfc.Dispose();
            label3.Font = font;
        }
    }
}
