
X = randn(2,10);          % make some input data
w = [1;1];                          % define a linear function
n = randn(1,10)*0.1;      % produce some noise
Y = w'*X + n;                   % produce outputs
scatter3(X(1,:),X(2,:),Y);
        

w = randn(2,1);
for iteration = 1 : 200              % in practice: until stopping criterion satisfied
    grad = 2*sum(repmat(w'*X-Y,size(X,1),1).*X,2);
    w = w - 0.001 * grad;
    err = sum((Y - w'*X).^2);      % just to check
    fprintf('err = %f\n', err);
end

hold on;    % keep old plot
Y_hat = w'*X;
w
scatter3(X(1,:),X(2,:),Y_hat, 'r');


