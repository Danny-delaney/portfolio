const cacheName = 'Danny-Delaney-09-PWA-Progressive-Web-Apps';
const filesToCache = [
  '.',
  'main.js',
  'index.html',
  'assets/css/screen.css',
  'assets/js/script.js',
  'assets/image/pixilart-sprite.png',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/main.js',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/index.html',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/assets/css/style.css',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/assets/js/script.js',
  './Danny-Delaney-09-PWA-Progressive-Web-Apps/assets/image/pixilart-sprite.png',

];

self.addEventListener('install', async e => {
  const cache = await caches.open(cacheName);
  await cache.addAll(filesToCache);
  return self.skipWaiting();
});

self.addEventListener('activate', e => {
  self.clients.claim();
});

self.addEventListener('fetch', async e => {
  const req = e.request;
  const url = new URL(req.url);

  if (url.origin === location.origin) {
    e.respondWith(cacheFirst(req));
  } else {
    e.respondWith(networkAndCache(req));
  }
});

async function cacheFirst(req) {
  const cache = await caches.open(cacheName);
  const cached = await cache.match(req.url);
  return cached || fetch(req);
}

async function networkAndCache(req) {
  const cache = await caches.open(cacheName);
  try {
    const fresh = await fetch(req);
    await cache.put(req, fresh.clone());
    return fresh;
  } catch (e) {
    const cached = await cache.match(req);
    return cached;
  }
}