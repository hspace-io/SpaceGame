import { json } from '@sveltejs/kit';
import { key2 } from '$env/static/private';

export function GET(event) {
	//console.log(...event.request.headers);
	const user_key2 = event.url.searchParams.get('key2');

	try {
		if (typeof user_key2 !== 'string') {
			throw new Error('key2 is not a string');
		}

		if (!key2.search(user_key2) && key2 === user_key2) {
			return json({
				key2: key2
			});
		}
	} catch (error) {
		return json({
			error: error.message,
		});
	}
	
	return json({
		// retrieve a specific header
		userAgent: event.request.headers.get('user-agent')
	});
}
