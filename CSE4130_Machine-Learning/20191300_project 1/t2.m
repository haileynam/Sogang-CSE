X = [randn(3,200)-ones(3,200)*1.8 ...
     randn(3,200)+ones(3,200)*1.8];     %produce some inputs
Y = [-1*ones(1,200), ones(1,200)];           %produce some labels
scatter3(X(1,:),X(2,:),X(3,:), 50, Y, 'filled');


w = randn(3,1);
Y_hat = sign(w'*X);
figure(2); scatter3(X(1,:),X(2,:),X(3,:),50,Y_hat, 'filled');
hold on;
plot3([0 w(1)], [0 w(2)], [0 w(3)], 'k');     %show w
hold off;
sum(Y_hat~=Y)/400               %error rate


for iteration = 1 : 100
    for i = 1 : size(X,2)
        if sign(w'*X(:,i)) ~= Y(i)
            w = w + X(:,i) * Y(i);
       end
    end
end
Y_hat = sign(w'*X);
sum(Y_hat~=Y)/400
