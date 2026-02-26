
const IP = "192.168.1.19"; // Update this!
const ros = new ROSLIB.Ros({ url: `ws://${IP}:9090` });

// Analog Steering (Left Stick)
const manager = nipplejs.create({
    zone: document.getElementById('steer-zone'),
    mode: 'dynamic', // Appears where you touch
    color: '#007aff',
    size: 110,
    lockY: true // Only Left/Right steering
});

let linear = 0, angular = 0;
const cmdVel = new ROSLIB.Topic({ ros, name: '/cmd_vel_web', messageType: 'geometry_msgs/Twist' });

function publish() {
    cmdVel.publish(new ROSLIB.Message({
        linear: { x: linear, y: 0, z: 0 },
        angular: { x: 0, y: 0, z: angular }
    }));
}

// Steering Logic: Precision Mapping
manager.on('move', (evt, data) => {
    // data.vector.x is mapped -1.0 to 1.0
    angular = -data.vector.x * 1.5; // Adjusted sensitivity
    publish();
});
manager.on('end', () => { angular = 0; publish(); });

// Pedal Logic: Throttle & Brake
let interval;
const startDrive = (v) => {
    if (interval) return;
    interval = setInterval(() => { linear = v; publish(); }, 50);
};
const stopDrive = () => { clearInterval(interval); interval = null; linear = 0; publish(); };

const tBtn = document.getElementById('throttle');
const bBtn = document.getElementById('brake');

tBtn.onpointerdown = () => startDrive(0.6);
tBtn.onpointerup = stopDrive;
bBtn.onpointerdown = () => startDrive(-0.4);
bBtn.onpointerup = stopDrive;

// Update Camera
document.getElementById('camera-stream').src = `http://${IP}:8080/stream?topic=/camera/image_raw`;