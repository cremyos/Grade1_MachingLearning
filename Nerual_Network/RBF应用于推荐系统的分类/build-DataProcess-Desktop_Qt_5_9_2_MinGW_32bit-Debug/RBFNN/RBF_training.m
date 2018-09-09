function [ W, sigma, C ] = RBF_training( data, label, n_center_vec )
%RBF_TRAINING Summary of this function goes here
%   Detailed explanation goes here

    % Using kmeans to find cinter vector
    rng(1);
    [idx, C] = kmeans(data, n_center_vec);

    % Calulate sigma 
    n_data = size(data,1);

    % calculate K
    K = zeros(n_center_vec, 1);
    for i=1:n_center_vec
        K(i) = numel(find(idx == i));
    end

    % Using knnsearch to find K nearest neighbor points for each center vector
    % then calucate sigma
    sigma = zeros(n_center_vec, 1);
    for i=1:n_center_vec
        [n] = knnsearch(data, C(i,:), 'k', K(i));
        L2 = (bsxfun(@minus, data(n,:), C(i,:)).^2);
        L2 = sum(L2(:));
        sigma(i) = sqrt(1/K(i)*L2);
    end
    % Calutate weights
    % kernel matrix
    k_mat = zeros(n_data, n_center_vec);

    for i=1:n_center_vec
        r = bsxfun(@minus, data, C(i,:)).^2;
        r = sum(r,2);
        k_mat(:,i) = exp((-r.^2)/(2*sigma(i)^2));
    end

    W = pinv(k_mat'*k_mat)*k_mat'*label;
end     