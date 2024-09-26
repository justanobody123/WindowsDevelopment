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
        public DateTimePicker DateTimePickerAlarm1
        {
            get => dateTimePickerAlarm1;
        }
        public DateTimePicker DateTimePickerAlarm2
        {
            get => dateTimePickerAlarm2;
        }
        public DateTimePicker DateTimePickerAlarm3
        {
            get => dateTimePickerAlarm3;
        }
        public DateTimePicker DateTimePickerAlarm4
        {
            get => dateTimePickerAlarm4;
        }
        public DateTimePicker DateTimePickerAlarm5
        {
            get => dateTimePickerAlarm5;
        }
        public CheckBox CheckBoxAlarm1
        {
            get => checkBoxAlarm1;
        }
        public CheckBox CheckBoxAlarm2
        {
            get => checkBoxAlarm2;
        }
        public CheckBox CheckBoxAlarm3
        {
            get => checkBoxAlarm3;
        }
        public CheckBox CheckBoxAlarm4
        {
            get => checkBoxAlarm4;
        }
        public CheckBox CheckBoxAlarm5
        {
            get => checkBoxAlarm5;
        }

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
            if (checkBoxAlarm1.Checked || checkBoxAlarm2.Checked || checkBoxAlarm3.Checked || checkBoxAlarm4.Checked || checkBoxAlarm5.Checked) 
            {
                parent.IsAlarmOn = true;
                //parent.NotifyIcon1.Text = "Будильник в " + this.dateTimePickerAlarm1.Text.ToString();
                //Можно застрингбилдить все активные будильники и отправить их в нотифайайкон
                StringBuilder builder = new StringBuilder();
                builder.Append("Будильник:");
                if (checkBoxAlarm1.Checked) { builder.Append("\nв " + dateTimePickerAlarm1.Text); }
                if (checkBoxAlarm2.Checked) { builder.Append("\nв " + dateTimePickerAlarm2.Text); }
                if (checkBoxAlarm3.Checked) { builder.Append("\nв " + dateTimePickerAlarm3.Text); }
                if (checkBoxAlarm4.Checked) { builder.Append("\nв " + dateTimePickerAlarm4.Text); }
                if (checkBoxAlarm5.Checked) { builder.Append("\nв "  + dateTimePickerAlarm5.Text); }
                parent.NotifyIcon1.Text = builder.ToString();
            }
            else
            {
                parent.IsAlarmOn = false;
                parent.NotifyIcon1.Text = "Clock";
            }

            //Console.WriteLine(parent.SAlarmDate);
            Hide();
        }
    }
}
