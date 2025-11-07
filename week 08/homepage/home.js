function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

document.getElementById("dontTellMalan").addEventListener("click", async function () {
    document.getElementById("dontTellMalan").style.backgroundColor = "#f00";
    await sleep(1000);
    document.getElementById("dontTellMalan").style.backgroundColor = "#2e2e2e";
});
