import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { PLYLoader } from 'three/addons/loaders/PLYLoader.js';
import { OBJLoader } from 'three/addons/loaders/OBJLoader.js';
import { GUI } from 'three/addons/libs/lil-gui.module.min.js';

import { default as createAlphaWrapModule } from 'AlphaWrap'
var AlphaWrap;

const windowSize = {
    width: window.innerWidth,
    height: window.innerHeight
}

const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );

const renderer = new THREE.WebGLRenderer();
renderer.setPixelRatio(window.devicePixelRatio);
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

let pointsToWrap;
let wrappedMeshObj = null;
let wrappedWireframeObj = null;
let lastWrappedPly = null;
let currentFileName = 'pointcloud';

const plyLoader = new PLYLoader();
const objLoader = new OBJLoader();

function computeAdaptivePointSize(geometry) {
    geometry.computeBoundingBox();
    const size = new THREE.Vector3();
    geometry.boundingBox.getSize(size);
    return size.length() * 0.004;
}

function loadPointGeometry(geometry) {
    // Remove previous point cloud
    if (pointsToWrap) {
        scene.remove(pointsToWrap);
    }
    clearWrappedMesh();

    const pointSize = computeAdaptivePointSize(geometry);
    const material = new THREE.PointsMaterial( { color: 0x888888, size: pointSize } );
    const points = new THREE.Points( geometry, material );
    pointsToWrap = points;
    scene.add(points);
}

function clearWrappedMesh() {
    if (wrappedMeshObj) {
        scene.remove(wrappedMeshObj);
        wrappedMeshObj = null;
    }
    if (wrappedWireframeObj) {
        scene.remove(wrappedWireframeObj);
        wrappedWireframeObj = null;
    }
    lastWrappedPly = null;
}

// Load default point cloud
plyLoader.load(
    'assets/torus_cube.ply',
    function (geometry) {
        loadPointGeometry(geometry);
    },
    (xhr) => {
        console.log((xhr.loaded / xhr.total) * 100 + '% loaded')
    },
    (error) => {
        console.log(error)
    }
)

camera.position.z = 50;

function animate() {
    requestAnimationFrame( animate );
    renderer.render( scene, camera );
}

animate();

const controls = new OrbitControls( camera, renderer.domElement );

window.addEventListener('resize', () => {
    windowSize.width = window.innerWidth
    windowSize.height = window.innerHeight
    camera.aspect = windowSize.width / windowSize.height
    camera.updateProjectionMatrix()
    renderer.setSize(windowSize.width, windowSize.height);
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
})

let alphaWrap;
createAlphaWrapModule().then(
    AlphaWrapModule => {
        AlphaWrap = AlphaWrapModule;
        alphaWrap = new AlphaWrap.AlphaWrap(
            alphaWrapParams.alpha,
            alphaWrapParams.offset
        );
    }
)

function feedPoints() {
    if (!pointsToWrap) return;
    alphaWrap.clearPoints();
    let pIdx = 0;
    let p = new Float32Array(3);
    for (let pointCoordinate of pointsToWrap.geometry.attributes.position.array) {
        p[pIdx++] = pointCoordinate;
        if (pIdx === 3) {
            alphaWrap.addPoint(p[0], p[1], p[2]);
            pIdx = 0;
        }
    }
}

function tryWrap() {
    if (!alphaWrap || !pointsToWrap) {
        console.log("Not ready: WASM or point cloud not loaded");
        return;
    }

    alphaWrapParams.wrapStatus = "Wrapping...";
    gui.controllersRecursive().forEach(c => c.updateDisplay());

    try {
        alphaWrap.setAlpha(alphaWrapParams.alpha);
        alphaWrap.setOffset(alphaWrapParams.offset);
        feedPoints();

        let s = alphaWrap.wrap();
        console.log("Wrap result:", s);

        clearWrappedMesh();

        let wrappedPlyString = alphaWrap.getWrappedMeshPly();
        lastWrappedPly = wrappedPlyString;

        let geometry = plyLoader.parse(wrappedPlyString);

        let meshMaterial = new THREE.MeshBasicMaterial( { color: 0xffff00 } );
        wrappedMeshObj = new THREE.Mesh( geometry, meshMaterial );

        let lineSegmentMaterial = new THREE.LineBasicMaterial( { color: 0x000000 } );
        let wireframe = new THREE.WireframeGeometry(geometry);
        wrappedWireframeObj = new THREE.LineSegments(wireframe, lineSegmentMaterial);

        wrappedMeshObj.visible = alphaWrapParams.showWrapped;
        wrappedWireframeObj.visible = alphaWrapParams.showWrapped;
        scene.add(wrappedMeshObj);
        scene.add(wrappedWireframeObj);

        alphaWrapParams.wrapStatus = "Done";
    } catch (e) {
        console.error("Wrap failed:", e);
        alphaWrapParams.wrapStatus = "Failed";
    }

    gui.controllersRecursive().forEach(c => c.updateDisplay());
}

function uploadPointCloud() {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.ply,.obj';
    input.onchange = (e) => {
        const file = e.target.files[0];
        if (!file) return;

        currentFileName = file.name.replace(/\.[^.]+$/, '');

        const reader = new FileReader();
        const ext = file.name.split('.').pop().toLowerCase();

        if (ext === 'ply') {
            reader.onload = (ev) => {
                const geometry = plyLoader.parse(ev.target.result);
                loadPointGeometry(geometry);
            };
            reader.readAsArrayBuffer(file);
        } else if (ext === 'obj') {
            reader.onload = (ev) => {
                const group = objLoader.parse(ev.target.result);
                // Extract geometry from the first mesh/points in the OBJ
                let geometry = null;
                group.traverse((child) => {
                    if (!geometry && child.geometry) {
                        geometry = child.geometry;
                    }
                });
                if (geometry) {
                    loadPointGeometry(geometry);
                } else {
                    console.error("No geometry found in OBJ file");
                }
            };
            reader.readAsText(file);
        }
    };
    input.click();
}

function downloadResult() {
    if (!lastWrappedPly) {
        console.log("No wrapped mesh to download. Run wrap first.");
        return;
    }
    const blob = new Blob([lastWrappedPly], { type: 'application/octet-stream' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `${currentFileName}-wrapped.ply`;
    a.click();
    URL.revokeObjectURL(url);
}

function clearCanvas() {
    if (pointsToWrap) {
        scene.remove(pointsToWrap);
        pointsToWrap = null;
    }
    clearWrappedMesh();
    currentFileName = 'pointcloud';
}

let alphaWrapParams = {
    alpha: 10,
    offset: 300,
    wrap: tryWrap,
    upload: uploadPointCloud,
    clear: clearCanvas,
    download: downloadResult,
    showWrapped: true,
    wrapStatus: "Not wrapped"
}

const gui = new GUI();
gui.add(alphaWrapParams, 'upload').name('Upload PLY/OBJ');
gui.add(alphaWrapParams, 'alpha', 0, 100);
gui.add(alphaWrapParams, 'offset', 0, 1000);
gui.add(alphaWrapParams, 'wrap');
gui.add(alphaWrapParams, 'download').name('Download Result');
gui.add(alphaWrapParams, 'clear').name('Clear');
gui.add(alphaWrapParams, 'showWrapped').name('Show Wrapped').onChange((v) => {
    if (wrappedMeshObj) wrappedMeshObj.visible = v;
    if (wrappedWireframeObj) wrappedWireframeObj.visible = v;
});
gui.add(alphaWrapParams, 'wrapStatus').name('Status').listen();
