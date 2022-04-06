from cgi import test
from cmath import sqrt
from random import shuffle
import math
import torch
import torch.nn as nn
import torchvision
import torch.utils.data as Data
import numpy as np
from torch.utils.data import Dataset, DataLoader, TensorDataset

total_test = 11
signal_raw = np.loadtxt('./data/signal_test_11.txt')
signal_raw = torch.from_numpy(signal_raw)
signal_reshape = signal_raw.reshape(total_test, 1, 100, 5)
signal = signal_reshape.transpose(2, 3)
signal = signal.float()
signal_v = torch.Tensor(signal)
yreal_raw = np.loadtxt('./data/yreal_test_11.txt')
yreal = yreal_raw.reshape(total_test, 1)
yreal_v = torch.Tensor(yreal)
yreal_v = 10000 * yreal_v
# print(signal_v.size())
# print(yreal_v.size())

test_data = TensorDataset(signal_v, yreal_v)
test_loader = Data.DataLoader(test_data, batch_size = 1, shuffle = False)

total_train = 550

signal_valid_raw = np.loadtxt('./data/signal_train_550.txt')
signal_valid_raw = torch.from_numpy(signal_valid_raw)
signal_valid_reshape = signal_valid_raw.reshape(total_train, 1, 100, 5)
signal_valid = signal_valid_reshape.transpose(2, 3)
signal_valid = signal_valid.float()
signal_valid_v = torch.Tensor(signal_valid)
yreal_valid_raw = np.loadtxt('./data/yreal_train_550.txt')
yreal_valid = yreal_valid_raw.reshape(total_train, 1)
yreal_valid_v = torch.Tensor(yreal_valid)
yreal_valid_v = 10000 * yreal_valid_v
# print(signal_valid_v.size())
# print(yreal_valid_v.size())
train_data = TensorDataset(signal_valid_v, yreal_valid_v)
train_loader = Data.DataLoader(train_data, batch_size = 1, shuffle = False)

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
net = torch.load('./LeNet.pkl', map_location = torch.device(device))
net.to(device)

torch.set_grad_enabled(False)
net.eval()

loss_function = nn.MSELoss()
#length = test_data.size(0)
acc = 0.0
test_running_loss = 0.0
valid_running_loss = 0.0

for i, data in enumerate(test_loader):
    x, y = data
    y_pred = net(x.to(device, torch.float))
    #loss = loss_function(y_pred, y.data)
    loss = (y_pred - y.data) * (y_pred - y.data)
    test_running_loss += float(loss.data)
    #pred = y_pred.data.argmax(dim = 1)
    #acc += (pred.data.cpu() == y.data).sum()
    # print('Test Predict:', y_pred, '| Ground Truth:', y.data)

for i, data in enumerate(train_loader):
    x, y = data
    y_pred = net(x.to(device, torch.float))
    #loss = loss_function(y_pred, y.data)
    loss = (y_pred - y.data) * (y_pred - y.data)
    valid_running_loss += float(loss.data)
    #pred = y_pred.data.argmax(dim = 1)
    #acc += (pred.data.cpu() == y.data).sum()
    # print('Validation Predict:', y_pred, '| Ground Truth:', y.data)
#acc = (acc / length) * 100
#print('Accuracy: %.2f' % acc, '%')
se_test = math.sqrt(test_running_loss / total_test)
se_valid = math.sqrt(valid_running_loss / total_train)
file_test = open('error.txt', 'w')
file_valid = open('valid_error.txt', 'w')
print(se_test, file = file_test)
print(se_valid, file = file_valid)
print('test standard error: %.4f' % se_test)
print('valid standard error: %.4f' % se_valid)