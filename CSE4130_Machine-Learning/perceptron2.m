function ret = perceptron1()

n_sample = 20;
% x_c1 = mvnrnd([0, 0], [1, .95; .95, 1.], n_sample);
% x_c2 = mvnrnd([1, 5], [1, .85; .85, 1.], n_sample);
% dlmwrite('x_c1', x_c1);
% dlmwrite('x_c2', x_c2);
x_c1 = dlmread('x_c1');
x_c2 = dlmread('x_c2');

% add bias term
x_c1 = [ones(n_sample,1) x_c1]; 
x_c2 = [ones(n_sample,1) x_c2];

% label
y1 = ones(n_sample,1)*-1; % -1 for c1
y2 = ones(n_sample,1);  % 1 for c2

% figure(1); xlim([-5 10]); ylim([-5 10])
% hold on
% plot(x_c1(:,2), x_c1(:,3), 'or'); 
% plot(x_c2(:,2), x_c2(:,3), '+b'); 
% hold off

x = [x_c1; x_c2];
y = [y1; y2];
% yx = repmat(y,1,3).*x; % yx

w = rand(1,3);

figure(1); xlim([-5 10]); ylim([-5 10]); axis equal;
drawDataHyperplane(x_c1, x_c2, w, 'k');



% w <- w+sum(misclassified yx), Fig(2)
n_epoch = 100;
eta = 0.001;
ret1 = Perceptron_mis_yx(y, x, w, n_epoch, eta); 
 
% w <- w+sum(squared error), Fig(3)
n_epoch = 100;
eta = 0.001;
ret2 = Perceptron_l2(y, x, w, n_epoch, eta);

% w <- w+sum(squared error) + lamda(w square), Fig(4)
n_epoch = 300;
eta = 0.001;
lamda = 0.001;
ret3 = Perceptron_l2_reg(y, x, w, n_epoch, eta, lamda);

fprintf('ret1 = %d, ret2 = %d, ret3 = %d \n', ret1, ret2, ret3);



function ret1 = Perceptron_mis_yx(y, x, w, n_epoch, eta)
    n_sample = size(y,1)/2;
    x_c1 = x(1:n_sample, :);
    x_c2 = x(n_sample+1:n_sample*2, :);
    yx = repmat(y,1,3).*x;
    
    fprintf('----- Perceptron_mis_yx() started -----\n');
    for e=1:n_epoch
    %     for i=1:n_sample*2
        wyx = dot(repmat(w,n_sample*2,1),yx,2);
    %     idx = find(wx2 < 0);
        yx_misclassified = yx(wyx < 0,:);
        w = w + sum(yx_misclassified);
%         w = w + eta*(sum(yx_misclassified)-2*0.001*w);

        err = sum(wyx < 0);
        fprintf('err = %d\n', err);
        figure(2); xlim([-5 10]); ylim([-5 10]); axis equal;
        hold on; drawDataHyperplane(x_c1, x_c2, w, 'k');
        hold off;
        
        if err == 0
            break;
        end
    end
    ret1 = e;

function ret2 = Perceptron_l2(y, x, w, n_epoch, eta)
    n_sample = size(y,1)/2;
    x_c1 = x(1:n_sample, :);
    x_c2 = x(n_sample+1:n_sample*2, :);
    
    fprintf('----- Perceptron_l2() started -----\n');
    for e=1:n_epoch
    %     for i=1:n_sample*2
        wx = dot(repmat(w,n_sample*2,1),x,2);
        diff = repmat((y - wx),1,3).*x;
        w = w + eta*sum(diff);
    %     w = w/max(abs(w));

        err1 = 1/2*sum((y - wx).^2);
        err2 = sum(y.*wx<0);
        fprintf('err(%d) = %f %d\n', e, err1, err2);
        figure(3); xlim([-5 10]); ylim([-5 10]); axis equal;
        hold on; drawDataHyperplane(x_c1, x_c2, w, 'k');
        hold off;
        
        if err2 == 0
            break;
        end
    end
    ret2 = e;

function ret3 = Perceptron_l2_reg(y, x, w, n_epoch, eta, lamda)
    n_sample = size(y,1)/2;
    x_c1 = x(1:n_sample, :);
    x_c2 = x(n_sample+1:n_sample*2, :);
    
    fprintf('----- Perceptron_l2_reg() started -----\n');
    for e=1:n_epoch
        wx = dot(repmat(w,n_sample*2,1),x,2);
        diff = repmat((y - wx),1,3).*x;
        w = w + eta*(sum(diff)-2*lamda*w*n_sample*2);

        err1 = 1/2*sum((y - wx).^2);
        err2 = sum(y.*wx<0);
        fprintf('err(%d) = %f %d\n', e, err1, err2);
        figure(4); xlim([-5 10]); ylim([-5 10]); axis equal;
        hold on; drawDataHyperplane(x_c1, x_c2, w, 'k');
        hold off;
        
        if err2 == 0
            break;
        end
    end
    ret3 = e;


function drawData(x_c1, x_c2)
    plot(x_c1(:,2), x_c1(:,3), '+b'); 
    plot(x_c2(:,2), x_c2(:,3), 'or'); 

function drawLine(w, col)    
    x = -5:.1:10;
    y = -w(1)/w(3)-w(2)/w(3)*x;
    
    plot(x, y, col); 
    
function drawDataHyperplane(x_c1, x_c2, w, col)
	hold on;
    drawData(x_c1, x_c2);
    drawLine(w, col);
    hold off;




