function [ y ] = RBF_predict( data, W, sigma, C )
%RBF_PREDICT Summary of this function goes here
%   Detailed explanation goes here
    n_data = size(data, 1);
    n_center_vec = size(C, 1);
    if numel(sigma) == 1
       sigma = repmat(sigma, n_center_vec, 1);
    end
    % kernel matrix
    k_mat = zeros(n_data, n_center_vec);
    for i=1:n_center_vec
        r = bsxfun(@minus, data, C(i,:)).^2;
        r = sum(r,2);
        k_mat(:,i) = exp((-r.^2)/(2*sigma(i)^2));
    end

    y = k_mat*W;
end