using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;

namespace Clock
{
    public partial class AlarmClock : Form
    {
        MainForm parent;
        public AlarmClock()
        {
            InitializeComponent();
        }
        public AlarmClock(MainForm parent) : this() 
        {
            this.parent = parent;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            Console.WriteLine(this.alarmTime.Text.ToString());
            parent.AlarmTime = this.alarmTime.Text.ToString();
            //Console.WriteLine(parent.SAlarmDate);
            Hide();
        }
    }
}
