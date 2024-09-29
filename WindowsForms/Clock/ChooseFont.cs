using Clock.Properties;
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
        MainForm parent;
        public ChooseFont()
        {
            InitializeComponent();
            //this.parent = parent;
            Console.WriteLine(Directory.GetCurrentDirectory());
            string[] ttfFiles = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.ttf");
            string[] otfFiles = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.otf");
            string[] files = new string[ttfFiles.Length + otfFiles.Length];
            ttfFiles.CopyTo(files, 0);
            otfFiles.CopyTo(files, ttfFiles.Length);
            for (int i = 0; i < ttfFiles.Length; i++)
            {
                comboBoxFonts.Items.Add(files[i].Split('\\').Last());
                Console.WriteLine($"Load font: {files[i]}");
            }
            //string[] extensions = new string[] { ".ttf", ".otf" };
            //string[] files = Directory.GetFiles(Directory.GetCurrentDirectory()).Where(item=> extensions.Contains(item.Split().Last('\\')));
        }
        public ChooseFont(MainForm owner) : this()
        {
            this.parent = owner;
        }
        private void comboBoxFonts_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetFontParameters();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (comboBoxFonts.SelectedIndex != -1) //If no item is selected, the SelectedIndex value is -1 https://learn.microsoft.com/en-us/dotnet/desktop/winforms/controls/combobox-control-overview-windows-forms?view=netframeworkdesktop-4.8#:~:text=If%20no%20item%20is%20selected%2C%20the%20SelectedIndex%20value%20is%20%2D1
            {
                PrivateFontCollection pfc = new PrivateFontCollection();
                Console.WriteLine(comboBoxFonts.SelectedItem.ToString());
                pfc.AddFontFile(comboBoxFonts.SelectedItem.ToString());
                Properties.Settings.Default.FontName = comboBoxFonts.SelectedItem.ToString();
                Console.WriteLine(Properties.Settings.Default.FontName);
                Properties.Settings.Default.FontSize = Convert.ToInt32(numericUpDownFontSize.Value);
                Properties.Settings.Default.Save();
                Font font = new Font(pfc.Families[0], Convert.ToInt32(numericUpDownFontSize.Value));
                pfc.Dispose();
                this.parent.LabelClock.Font = font;
            }
        }
        void SetFontParameters()
        {
            PrivateFontCollection pfc = new PrivateFontCollection();
            pfc.AddFontFile(comboBoxFonts.SelectedItem.ToString());
            Font font = new Font(pfc.Families[0], Convert.ToInt32(numericUpDownFontSize.Value));
            pfc.Dispose();
            label3.Font = font;
        }


        private void button1_Click(object sender, EventArgs e)
        {
            SetFontParameters();
        }
    }
}
