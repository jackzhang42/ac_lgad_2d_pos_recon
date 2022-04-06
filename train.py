from random import shuffle
import torch
import torchvision
import torch.nn as nn
import torch.utils.data as Data
from torch.utils.data import Dataset, DataLoader, TensorDataset
import numpy as np
import sys

from model import LeNet
model = LeNet()

Epoch = int(sys.argv[2])
batch_size = int(sys.argv[1])
# lr = 0.001
lr = 0.001

# train_data = torchvision.datasets.MNIST(root = './data', train = True, transform = torchvision.transforms.ToTensor(), download = False)
# train_loader = Data.DataLoader(train_data, batch_size = batch_size, shuffle = True, num_workers = 0, drop_last = True)

total = 550

signal_raw = np.loadtxt('./data/signal_train_550.txt')
signal_raw = torch.from_numpy(signal_raw)
signal_reshape = signal_raw.reshape(total, 1, 100, 5)
signal = signal_reshape.transpose(2, 3)
signal = signal.float()
signal_v = torch.Tensor(signal)
yreal_raw = np.loadtxt('./data/yreal_train_550.txt')
yreal = yreal_raw.reshape(total, 1)
yreal_v = torch.Tensor(yreal)
yreal_v = 10000 * yreal_v
# print(signal_v.size())
# print(yreal_v.size())
train_data = TensorDataset(signal_v, yreal_v)
train_loader = Data.DataLoader(train_data, batch_size = batch_size, shuffle = True)

loss_function = nn.MSELoss()
optimizer = torch.optim.Adam(model.parameters(), lr = lr)

torch.set_grad_enabled(True)
model.train()

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model.to(device)

for epoch in range(Epoch):
    running_loss = 0.0
    acc = 0.0
    for step, data in enumerate(train_loader):
        x, y = data
        optimizer.zero_grad()
        y_pred = model(x.to(device, torch.float))
        loss = loss_function(y_pred, y.to(device, torch.float))
        loss.backward()
        running_loss += float(loss.data.cpu())
        #pred = y_pred.argmax(dim = 1)
        #acc += (loss < 1.0).sum()
        optimizer.step()
        if step % 10 == 0:
            loss_avg = running_loss / (step + 1)
            acc_avg = float(acc / ((step + 1) * batch_size))
            #print('Epoch', epoch + 1, ', step', step + 1, '| Loss_avg: %.4f' % loss_avg, '| Accuracy: %.4f' % acc_avg)
            print('Epoch', epoch + 1, ', step', step + 1, '| Loss_avg: %.4f' % loss_avg)

torch.save(model, './LeNet.pkl')