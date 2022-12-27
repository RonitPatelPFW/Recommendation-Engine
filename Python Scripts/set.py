import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.feature_extraction.text import CountVectorizer

booksdata = pd.read_csv('/Users/Home/Deskstop/Projects/Recommendation-Engine/BookDataSet/BX-Books.csv', error_bad_lines=False, encoding='latin-1', sep=';')
booksdata = booksdata.drop_duplicates(subset='Book-Title')
sampleSize = 15000
booksdata = booksdata.sample(n=sampleSize, replace=False, random_state=490)
booksdata = booksdata.reset_index()
booksdata = booksdata.drop('index', axis=1)


def cleanText(author):
    return str(author).lower()

booksdata['Book-Author'] = booksdata['Book-Author'].apply(cleanText)
booksdata['Publisher'] = booksdata['Publisher'].apply(cleanText)
booksdata['Book-Title'] = booksdata['Book-Title'].apply(cleanText)
booksdata2 = booksdata.drop(['ISBN', 'Image-URL-S', 'Image-URL-M', 'Image-URL-L', 'Year-Of-Publication'], axis=1)

booksdata2['data'] = booksdata2[booksdata2.columns[1:]].apply(
    lambda x: ' '.join(x.dropna().astype(str)), 
    axis=1
)

vectorizer = CountVectorizer()
vectorized = vectorizer.fit_transform(booksdata2['data'])
similarity = cosine_similarity(vectorized)

booksdata = pd.DataFrame(similarity, columns=booksdata['Book-Title'], index=booksdata['Book-Title']).reset_index()

input_book = 'far beyond the stars (star trek deep space nine)'
recommendations = pd.DataFrame(booksdata.nlargest(5,input_book)['Book-Title'])
recommendations = recommendations[recommendations['Book-Title']!=input_book]

print(recommendations)