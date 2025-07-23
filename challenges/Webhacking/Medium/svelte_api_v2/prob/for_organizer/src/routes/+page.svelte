<script>
	export let data;
</script>

<h1>Hello World!</h1>

<code>{data.result}</code>

<p>{data.uuid}</p>

<form method="POST">
	<input type="text" name="url" />
	<input type="submit" value="Submit" />
</form>
