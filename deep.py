import pandas as pd
import numpy as np
from keras.layers import Embedding, Flatten, Dense, Concatenate
from keras.models import Sequential

#load in the data
data = pd.read_csv('ratings.csv')

#create lists of all unique users and items
users = data['user_id'].unique()
items = data['item_id'].unique()

#create dictionaries to map the user and item IDs to integer indices
user_to_index = {user: index for index, user in enumerate(users)}
item_to_index = {item: index for index, item in enumerate(items)}

#create a list of tuples with the mapped indices
ratings = [(user_to_index[row['user_id']], item_to_index[row['item_id']], row['rating']) for _, row in data.iterrows()]

#create the model
model = Sequential()

#add the embedding layers for users and items
model.add(Embedding(len(users), 50, input_length=1))
model.add(Embedding(len(items), 50, input_length=1))

#flatten the embeddings and concatenate them
model.add(Flatten())
model.add(Concatenate())

#add the fully connected layer
model.add(Dense(256, activation='relu'))

#add the output layer
model.add(Dense(1))

#compile the model
model.compile(loss='mean_squared_error', optimizer='adam')

#split the data into training and testing sets
np.random.shuffle(ratings)
split_index = int(len(ratings) * 0.8)
train_ratings = ratings[:split_index]
test_ratings = ratings[split_index:]

#create numpy arrays for the inputs and labels
train_users = np.array([rating[0] for rating in train_ratings])
train_items = np.array([rating[1] for rating in train_ratings])
train_labels = np.array([rating[2] for rating in train_ratings])

test_users = np.array([rating[0] for rating in test_ratings])
test_items = np.array([rating[1] for rating in test_ratings])
test_labels = np.array([rating[2] for rating in test_ratings])

#fit the model
model.fit([train_users, train_items], train_labels, epochs=5)

#evaluate the model
loss = model.evaluate([test_users, test_items], test_labels)
print(f'Test loss: {loss}')