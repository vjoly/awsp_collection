%%Load excel tables %%

filedata1 = 'test_0_pos.xlsx';
%filedata2 = 'fakedata2.xlsx';
%filedata3 = 'fakedata3.xlsx';

%% Data definitions

inertia =  5.452985; %inertia around z
time = xlsread(filedata1,'J1:J182'); %time vector in seconds

%position1= xlsread(filedata1,'B2:B32'); %Position vector in test 1. extracts the data from the specified range in a column vector.
%position2= xlsread(filedata2,'B2:B32'); %Position vector in test 2
%position3= xlsread(filedata3,'B2:B32'); %Position vector in test 3.
%Position = [position1 position2 position3]; %Position matrix for all three tests

vel1= xlsread(filedata1,'I1:I182'); %Velocity vector in test 1
% vel2= xlsread(filedata2,'C2:C32'); %Velocity vector in test 2
% vel3= xlsread(filedata3,'C2:C32'); %Velocity vector in test 3
% Velocity =[vel1 vel2 vel3]; %Velocity matrix for all three tests

%% Fitting the exponential to the test data points

%Vel_mean = mean([Velocity], 2); %Average values from the velocity tests
ExpFit = fit(time,vel1,'exp1') %The "fit" function fits an exponential from the form a*exp(b*x) to data points

ExpValues = coeffvalues(ExpFit); %Extracts the coefficients "a" and "b" from the form "a*exp(b*x)" into a vector;
ExpExpression = ExpValues(1)*exp(ExpValues(2)*time); %Populating the form "a*exp(b*x)" with the coefficients

Damping = -inertia*ExpValues(2) %Damping in surge, sway and yaw is defined by m/s, where s corresponds to -b from the form "a*exp(b*x)"

plot(time, ExpExpression, 'LineWidth', 2) %Plotting the fit
hold on;
plot (time, vel1) %Plotting the test results
