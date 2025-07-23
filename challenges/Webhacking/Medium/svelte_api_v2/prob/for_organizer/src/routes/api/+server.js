import { json } from '@sveltejs/kit';

export function GET() {
	return json({
		text: 'API v2 Service'
	});
}
