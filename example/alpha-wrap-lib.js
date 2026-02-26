import { default as createAlphaWrapModule } from './scripts/alpha_wrap.js';

let _modulePromise = null;
function getModule() {
    if (!_modulePromise) _modulePromise = createAlphaWrapModule();
    return _modulePromise;
}

export async function createAlphaWrap(alpha = 10, offset = 300) {
    const mod = await getModule();
    return new mod.AlphaWrap(alpha, offset);
}
