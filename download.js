const download = require('download');
const ProgressBar = require('progress');
const fs = require('fs');

if (fs.existsSync('./deezer-sdk')) process.exit();

const SDK_URL = 'https://build-repo.deezer.com/native_sdk/deezer-native-sdk-v1.2.10.zip';
const EXTRACT_DIR = 'deezer-sdk';

const progress = new ProgressBar('  downloading Deezer SDK :  [:bar] :rate/kbps :percent :etas', {
  complete: '#',
  incomplete: '-',
  width: 20,
  total: 0
});

download(SDK_URL, EXTRACT_DIR, {
  extract: true,
  strip: 2,
  filter: ({ path }) => path.startsWith('Bins/') || path.startsWith('Include/')
}).on('response', res => {
  progress.total = res.headers['content-length'] / 1000;
  res.on('data', data => progress.tick(data.length / 1000));
}).then(() => console.log('\n')).catch(error => {
  console.log('\n');
  console.error(error);
});