in = rand(128,1)+i*rand(128,1);

out = mod16QAM(in, {{'block_length',2},{'another_parameter',3.3}});

plot(1:128,abs(in),1:128,abs(out));
