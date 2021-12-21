
async function getVersion() {
    const response = await fetch('http://localhost:3000/api/version', {
        mode: 'cors',
    }) 
    .then(response => {
        return response.text()
    })
    .then((data) => {
        console.log (data ? JSON.parse(data) : {})
    })
    .catch((error) => {
        console.log(error)
    })

    console.log('Get Version');
}