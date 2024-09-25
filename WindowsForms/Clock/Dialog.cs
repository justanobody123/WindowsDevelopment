using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Media;
using System.IO;
using System.Threading;

namespace Clock
{
    public partial class Dialog : Form
    {
        SoundPlayer player;
        Thread alarmThread;
        MainForm parent;
        public Dialog()
        {
            InitializeComponent();
            player = new SoundPlayer("..\\Media\\alarm_clock_sound.wav");

        }
        public Dialog(MainForm parent) : this()
        {
            this.parent = parent;
        }
        private void Dialog_Load(object sender, EventArgs e)
        {
            alarmThread = new Thread(PlaySound);
            alarmThread.IsBackground = true;
            alarmThread.Start();
        }
        private void PlaySound()
        {
            player.PlayLooping();
        }
        private void Dialog_FormClosing(object sender, FormClosingEventArgs e)
        {
            player.Stop();
            alarmThread.Join();
        }
    }
}
