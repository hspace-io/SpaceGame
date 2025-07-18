window.onload = () => {
    const imageDisplay = document.getElementById('imageDisplay');
    const urlSearch = new URLSearchParams(location.search);
    const imgPath = urlSearch.get('imageDisplay') ?? DEFAUL_IMG_URL;
    const imgSrc = new URL(imgPath, location.origin);
    imageDisplay.src = imgSrc;

    const form = document.querySelector('form');
    form.addEventListener('submit', (e) => {
        e.preventDefault();
        const image_url = document.getElementById('image_url').value;
        fetch(`/update_profile?session=${urlSearch.get('session')}`, {
            method: "POST",
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ image_url: image_url }),
        });
        console.log(image_url)
    })
}