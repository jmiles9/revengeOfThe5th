%257 version
%12.5

clear all;

L = 0.3;
r = 0.01279;
k = 200;
c = 900;
p = 2700;

% ambient temp
Ta = 295.15;

kc = 5;
Vacross = 12.5;
PowerResistor = 15;
PowerOn = Vacross / PowerResistor;
Pin = PowerOn;

sigma = 5.67 * 10^(-8);
epsilon = 1;

% length resoultion, in meters
dx = 0.005;
% time resolution, in seconds
dt = 0.1;
% total time
t = 10000;

% array used for numerical calculation
T = 0:dx:L;

% how often we poll sensors
pollTime = 3;

% how often we switch power
powerTime = 500;

% creates sensor arrays
x = 0:3:t;
S1 = size(x);
S2 = size(x);
S3 = size(x);
S4 = size(x);
S5 = size(x);
% locoation of sensors in bar
s1loc = round(0.0163,3);
s2loc = round(0.0842,3);
s3loc = round(0.1520,3);
s4loc = round(0.2200,3);
s5loc = round(0.2880,3);

n = size(T,2);

% initializes bar to room temp
for i=1:n
    T(i) = Ta;
end

lastpoll = 1;
lastSwitch = 1;
sensorArrayIndex = 1;


for i = 1:t/dt
    % heated end power input
    T(1) = Pin * dt / ( c * p * pi * r^2 * dx ) + T(1);
    % convection radiation from middle
    T(2:n-1) = T(2:n-1) - 2*kc*(T(2:n-1)-Ta)*dt / (c * p * r);
    % thermal radiation from middle
    T(2:n-1) = T(2:n-1) - 2*epsilon*sigma*(T(2:n-1).^4-Ta^4)*dt / (c * p * r);

    % extra convection from ends
    T(1) = T(1) - kc*(T(1)-Ta)*dt / (c * p * dx) - 2*kc*(T(1)-Ta)*dt / (c * p * r);
    T(n) = T(n) - kc*(T(n)-Ta)*dt / (c * p * dx) - 2*kc*(T(n)-Ta)*dt / (c * p * r);
    % extra thermal radiation from ends
    T(1) = T(1) - epsilon*sigma*(T(1)^4-Ta^4)*dt / (c * p * dx) - 2*epsilon*sigma*(T(1).^4-Ta^4)*dt / (c * p * r);
    T(n) = T(n) - epsilon*sigma*(T(n)^4-Ta^4)*dt / (c * p * dx) - 2*epsilon*sigma*(T(n).^4-Ta^4)*dt / (c * p * r);

    % heated end conduction
    T(1) = T(1) - dt * k * (T(1) - T(2)) / (c * p * dx^2);
    % middle conduction
    T(2:n-1) = T(2:n-1) + k / (c * p) * dt * (T(1:n-2) - 2*T(2:n-1) + T(3:n))/dx^2;
    % unheated end conduction
    T(n) = T(n) + dt * k * (T(n-1) - T(n)) / (c * p * dx^2);
    if i * dt > lastpoll + pollTime
       S1( sensorArrayIndex ) = T(cast(s1loc * 200,'int16'));
       S2( sensorArrayIndex ) = T(cast(s2loc * 200,'int16'));
       S2( sensorArrayIndex ) = T(cast(s2loc * 200,'int16'));
       S3( sensorArrayIndex ) = T(cast(s3loc * 200,'int16'));
       S4( sensorArrayIndex ) = T(cast(s4loc * 200,'int16'));
       S5( sensorArrayIndex ) = T(cast(s5loc * 200,'int16'));
       sensorArrayIndex = sensorArrayIndex + 1;
       lastpoll = i * dt;
    end
    if i * dt > lastSwitch + powerTime 
        if Pin > 0
            Pin = 0;
        else 
            Pin = PowerOn;
        end
        lastSwitch = i * dt;
    end
end
hold on
plot(S1);
plot(S2);
plot(S3);
plot(S4);
plot(S5);
title('Steady-State Temperature vs Time for Matte Black Aluminum Rod')
xlabel('Distance Along Rod (shifted one cm right) (cm)') % x-axis label
ylabel('Temperature (K)') % y-axis label

