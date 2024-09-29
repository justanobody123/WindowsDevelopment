using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Clock
{
    public partial class TimerForm : Form
    {
        MainForm parent;
        public TimerForm()
        {
            InitializeComponent();
        }
        public DateTimePicker DateTimePickerTimer
        {
            get => dateTimePickerTimer;
        }
        public TimerForm(MainForm parent) : this()
        {
            this.parent = parent;
        }
        private void btnOk_Click(object sender, EventArgs e)
        {
            if (dateTimePickerTimer.Text != "0:00:00")
            {
                parent.TimerSettings = DateTime.Now;
                Console.WriteLine(parent.TimerSettings.ToString());
                Console.WriteLine(dateTimePickerTimer.Value.ToString());
                parent.LabelTimer.Text = dateTimePickerTimer.Text;
                parent.TimerSettings = parent.TimerSettings.AddHours(dateTimePickerTimer.Value.Hour);
                parent.TimerSettings = parent.TimerSettings.AddMinutes(dateTimePickerTimer.Value.Minute);
                parent.TimerSettings = parent.TimerSettings.AddSeconds(dateTimePickerTimer.Value.Second);
                Console.WriteLine(parent.TimerSettings.ToString());
                parent.LabelTimer.Visible = parent.ControlsVisible;
                parent.HiddenTimer.Start();
                //dateTimePickerTimer.Value
                this.Hide();
            }
        }
    }
}
