function res = svm1()

n_sample = 20;
% c1 = mvnrnd([1, 5], [1, .85; .85, 1.], n_sample);
% c2 = mvnrnd([0, 0], [1, .95; .95, 1.], n_sample);
% dlmwrite('c1', c1);
% dlmwrite('c2', c2);
x_c1 = dlmread('x_c1');
x_c2 = dlmread('x_c2');

x_c1 = [ones(n_sample,1) x_c1];
x_c2 = [ones(n_sample,1) x_c2];

y1 = ones(n_sample,1)*-1;
y2 = ones(n_sample,1);


x = [x_c1; x_c2];
y = [y1; y2];
yx = repmat(y,1,3).*x; % yx



n_epoch = 300;
eta = 0.001;

lamda = 0.01;
w = rand(1,3); % [b w1 w2]


figure(5); xlim([-5 10]); ylim([-5 10]); axis equal;
hold on; drawDataHyperplane(x_c1, x_c2, w, 'k')
hold off;
for e=1:n_epoch
    wyx = dot(repmat(w,n_sample*2,1),yx,2);
    
%     err_idx = wyx<0;
    err_idx = wyx<1;
    
%     w = w + eta*(-2*lamda*w + sum(yx(err_idx,:)));
    w = w + eta*(-2*lamda*w*(n_sample*2) + sum(yx(err_idx,:)));
%     w = w + eta*(sum(yx(err_idx,:)));
     
    err1 = sum(err_idx);
    err2 = sum(wyx(err_idx));
    fprintf('err(%d) = %f %d, w= %f %f %f, |w|=%f, |w23|=%f\n', e, err1, err2, w, sqrt(sum(w.^2)), sqrt(sum(w(2:3).^2)));
    
    figure(5); xlim([-5 10]); ylim([-5 10]); axis equal;
    hold on; drawDataHyperplane(x_c1, x_c2, w, 'k');
    hold off;
    1;
end

%     figure(3); xlim([-5 10]); ylim([-5 10]); axis equal;
%     hold on; drawDataHyperplane(x_c1, x_c2, w, 'k');
%     hold off;


function drawData(c1, c2)
    plot(c1(:,2), c1(:,3), '+b'); 
    plot(c2(:,2), c2(:,3), 'or'); 

function drawLine(w, col)
    x = -5:.1:10;
    y = -w(1)/w(3)-w(2)/w(3)*x;
    
    plot(x, y, col); 
    
function drawDataHyperplane(x_c1, x_c2, w, col)
	hold on;
    drawData(x_c1, x_c2);
    drawLine(w, col);
    hold off;


